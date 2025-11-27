#include <stdio.h>          // printf, scanf, perror
#include <stdlib.h>         // exit, malloc, free
#include <fcntl.h>          // open
#include <unistd.h>         // read, lseek, close
#include <stdint.h>         // uint8_t, uint32_t, uint64_t
#include <math.h>           // pow
#include <inttypes.h>       // PRIu64
#include "exFATstruct.h" 

// ------------------ Estructuras auxiliares SOLO en este .c ------------------

// Entrada principal de archivo (0x85)
typedef struct {
    uint8_t  entryType;        // 0x85
    uint8_t  secondaryCount;   // Número de entradas secundarias
    uint16_t setChecksum;      // Checksum del conjunto
    uint16_t fileAttributes;   // Atributos del archivo
    uint16_t reserved1;        // Reservado
    uint32_t createTS;         // Timestamp creación
    uint32_t modifyTS;         // Timestamp modificación
    uint32_t accessTS;         // Timestamp acceso
    uint8_t  createTZ;         // Zona horaria creación
    uint8_t  modifyTZ;         // Zona horaria modificación
    uint8_t  accessTZ;         // Zona horaria acceso
    uint8_t  reserved2[7];     // Reservado
} __attribute__((packed)) FileDirEntry;

// Entrada Stream Extension (0xC0)
typedef struct {
    uint8_t  entryType;        // 0xC0
    uint8_t  generalFlags;     // Flags
    uint8_t  reserved1;        // Reservado
    uint8_t  nameLength;       // Longitud del nombre
    uint16_t nameHash;         // Hash del nombre
    uint16_t reserved2;        // Reservado
    uint64_t validDataLength;  // Tamaño válido del archivo (bytes)
    uint32_t reserved3;        // Reservado (offset 16-19)
    uint32_t firstCluster;     // Primer cluster (offset 20-23)
    uint64_t dataLength;       // Tamaño (offset 24-31)
} __attribute__((packed)) StreamExtEntry;

// -----------------------------------------------------------------------------
// Función auxiliar: lee la entrada FAT de un cluster y devuelve su valor
// -----------------------------------------------------------------------------
uint32_t leerEntradaFAT(int fd, exFatBootSector *boot,
                        int bytesPerSector, uint32_t cluster) {

    // Calculamos el offset (en bytes) donde comienza la FAT
    off_t fatStartByte = (off_t)boot->FATOffset * bytesPerSector;

    // Cada entrada de FAT ocupa 4 bytes, multiplicamos por el número de cluster
    off_t entryOffset = fatStartByte + (off_t)cluster * 4;

    // Movemos el puntero del archivo hasta la entrada
    if (lseek(fd, entryOffset, SEEK_SET) < 0) {
        perror("Error al hacer lseek en la FAT");
        exit(1);   // En un laboratorio podemos terminar el programa
    }

    // Leemos 4 bytes (un uint32_t) de la FAT
    uint32_t value = 0;
    if (read(fd, &value, sizeof(uint32_t)) != sizeof(uint32_t)) {
        perror("Error al leer entrada de la FAT");
        exit(1);
    }

    // Devolvemos el valor leído
    return value;
}

// -----------------------------------------------------------------------------
// Función que muestra la cadena de clusters de un archivo usando la FAT
// -----------------------------------------------------------------------------
void mostrarCadenaClusters(int fd, exFatBootSector *boot,
                           int bytesPerSector, uint32_t firstCluster) {

    // Si el primer cluster es 0, el archivo no tiene datos
    if (firstCluster == 0) {
        printf("  (Archivo sin clusters asignados: firstCluster = 0)\n");
        return;
    }

    // Variable para el cluster actual
    uint32_t current = firstCluster;

    printf("  Cadena de clusters: ");

    // Recorremos hasta encontrar fin de cadena o un valor inválido
    while (1) {
        // Mostramos el cluster actual
        printf("%u ", current);

        // Leemos la entrada FAT del cluster actual
        uint32_t fatValue = leerEntradaFAT(fd, boot, bytesPerSector, current);

        // Condiciones de fin de cadena en exFAT (simplificado):
        // 0x00000000 -> libre (algo raro si estamos en la cadena)
        // 0xFFFFFFFF o valores altos -> fin de cadena
        if (fatValue == 0x00000000 ||
            fatValue == 0xFFFFFFFF ||
            fatValue >= 0xFFFFFFF8) {
            // Terminamos la cadena
            break;
        }

        // Actualizamos current con el siguiente cluster
        current = fatValue;
    }

    printf("\n");
}

// -----------------------------------------------------------------------------
// Programa principal para TAREA 4
// -----------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    // Verificamos que se pase el nombre de la imagen como argumento
    if (argc != 2) {
        printf("Uso: %s <imagen.exfat>\n", argv[0]);
        exit(1);
    }

    // Abrimos la imagen exFAT en modo solo lectura
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("No se pudo abrir la imagen");
        exit(1);
    }

    // Leemos el boot sector en la estructura definida en tu .h
    exFatBootSector boot;
    if (read(fd, &boot, sizeof(boot)) != sizeof(boot)) {
        perror("No se pudo leer el boot sector");
        close(fd);
        exit(1);
    }

    // Calculamos bytes por sector y sectores por cluster (2^n)
    int bytesPerSector    = (int)pow(2.0, (double)boot.BytePerSector);
    int sectorsPerCluster = (int)pow(2.0, (double)boot.SectorPerCluster);
    int bytesPerCluster   = bytesPerSector * sectorsPerCluster;

    // Calculamos el índice del cluster del directorio raíz (respecto al cluster 2)
    int rootIdx = boot.RootDirFirstCluster - 2;

    // Sector donde empieza el cluster del directorio raíz
    int rootStartSector = boot.ClusterHeapOffset +
                          rootIdx * sectorsPerCluster;

    // Offset en bytes de ese sector dentro del archivo
    off_t rootStartByte = (off_t)rootStartSector * bytesPerSector;

    // Reservamos memoria para leer un cluster completo del directorio raíz
    unsigned char *clusterBuf = (unsigned char *)malloc(bytesPerCluster);
    if (!clusterBuf) {
        perror("malloc");
        close(fd);
        exit(1);
    }

    // Posicionamos el puntero de archivo al inicio del cluster del root
    if (lseek(fd, rootStartByte, SEEK_SET) < 0) {
        perror("Error en lseek al root");
        free(clusterBuf);
        close(fd);
        exit(1);
    }

    // Leemos el cluster completo en memoria
    if (read(fd, clusterBuf, bytesPerCluster) != bytesPerCluster) {
        perror("No se pudo leer el directorio raiz");
        free(clusterBuf);
        close(fd);
        exit(1);
    }

    // Calculamos cuántas entradas de 32 bytes caben en un cluster
    int entries = bytesPerCluster / 32;

    printf("Entradas del directorio raiz:\n");

    // Recorremos todas las entradas del directorio raíz
    for (int i = 0; i < entries; i++) {

        // Puntero a la entrada i-ésima
        uint8_t *entry = clusterBuf + i * 32;

        // Si entryType es 0x00, ya no hay más entradas válidas
        if (entry[0] == 0x00)
            break;

        // Si es 0x85, es una File Directory Entry (entrada principal)
        if (entry[0] == 0x85) {

            // Interpretamos la entrada actual como FileDirEntry
            FileDirEntry *f = (FileDirEntry *)entry;

            // Verificamos que exista al menos una entrada más (la 0xC0)
            if (i + 1 >= entries)
                break;

            // La siguiente entrada debería ser Stream Extension (0xC0)
            uint8_t *nextEntry = clusterBuf + (i + 1) * 32;
            StreamExtEntry *s  = (StreamExtEntry *)nextEntry;

            if (s->entryType != 0xC0) {
                // Si no es 0xC0, algo no cuadra; la saltamos
                continue;
            }

            // Mostramos información básica del archivo
            printf("\nArchivo encontrado:\n");
            printf(" - Nombre corto: FILE_%d\n", i);               // nombre corto ficticio
            printf(" - Tamaño: %" PRIu64 " bytes\n", s->validDataLength);
            printf(" - Primer cluster: %u\n", s->firstCluster);

            // Preguntamos si queremos mostrar la cadena de clusters
            char resp;
            printf("¿Desea mostrar la cadena de clusters? (s/n): ");
            fflush(stdout);                         // Aseguramos que se imprima
            scanf(" %c", &resp);                    // Leemos un carácter, ignorando espacios previos

            if (resp == 's' || resp == 'S') {
                // Llamamos a la función que recorre la FAT e imprime la cadena
                mostrarCadenaClusters(fd, &boot, bytesPerSector, s->firstCluster);
            }

            // Avanzamos una entrada adicional porque ya procesamos también la 0xC0
            i++;
        }
    }

    // Liberamos la memoria del buffer
    free(clusterBuf);

    // Cerramos el archivo de la imagen
    close(fd);

    // Terminamos correctamente
    return 0;
}
