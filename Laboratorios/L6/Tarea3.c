
#include <stdio.h>      
#include <stdlib.h>     
#include <fcntl.h>      
#include <unistd.h>     
#include <stdint.h>     
#include <math.h>
#include <inttypes.h>
#include "exFATstruct.h"       

// Tu exFatBootSector viene del .h (no se modifica)
// #include "tu_header.h"

// ------------------ Estructuras auxiliares (en .c) -------------------

typedef struct {
    uint8_t entryType;        // 0x85
    uint8_t secondaryCount;
    uint16_t setChecksum;
    uint16_t fileAttributes;
    uint16_t reserved1;
    uint32_t createTS;
    uint32_t modifyTS;
    uint32_t accessTS;
    uint8_t createTZ;
    uint8_t modifyTZ;
    uint8_t accessTZ;
    uint8_t reserved2[7];
} __attribute__((packed)) FileDirEntry;

typedef struct {
    uint8_t entryType;      // 0xC0
    uint8_t generalFlags;
    uint8_t reserved1;
    uint8_t nameLength;
    uint16_t nameHash;
    uint16_t reserved2;
    uint64_t validDataLength;  // tamaño archivo
    uint32_t reserved3;
    uint32_t firstCluster;      // primer cluster
    uint64_t dataLength;
} __attribute__((packed)) StreamExtEntry;

// ---------------------------------------------------------------------

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s <imagen.exfat>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("No se pudo abrir imagen");
        exit(1);
    }

    // 1. Leer boot sector
    exFatBootSector boot;

    if (read(fd, &boot, sizeof(boot)) != sizeof(boot)) {
        perror("Error leyendo boot sector");
        close(fd);
        exit(1);
    }

    int bytesPerSector = pow(2, boot.BytePerSector);
    int sectorsPerCluster = pow(2, boot.SectorPerCluster);
    int bytesPerCluster = bytesPerSector * sectorsPerCluster;

    // 2. Calcular dónde está el cluster del root directory
    int rootIdx = boot.RootDirFirstCluster - 2;
    int rootStartSector = boot.ClusterHeapOffset +
                          rootIdx * sectorsPerCluster;
    off_t rootStartByte = (off_t)rootStartSector * bytesPerSector;

    // 3. Leer el cluster completo
    unsigned char *clusterBuf = malloc(bytesPerCluster);
    if (!clusterBuf) {
        perror("malloc");
        close(fd);
        exit(1);
    }

    lseek(fd, rootStartByte, SEEK_SET);
    read(fd, clusterBuf, bytesPerCluster);

    // 4. Procesar entradas del directorio raíz de 32 bytes
    int entries = bytesPerCluster / 32;

    printf("Entradas del directorio raiz:\n");

    for (int i = 0; i < entries; i++) {

        uint8_t *entry = clusterBuf + i*32;

        if (entry[0] == 0x00)
            break; // No hay más entradas válidas

        if (entry[0] == 0x85) {   // File Directory Entry

            // cast seguro
            FileDirEntry *f = (FileDirEntry *)entry;

            // La entrada siguiente debería ser 0xC0
            StreamExtEntry *s = (StreamExtEntry *)(entry + 32);

            if (s->entryType != 0xC0) {
                // imagen mal formada, pero seguimos
                continue;
            }

            printf("\nArchivo encontrado:\n");
            printf(" - Nombre corto: FILE_%d\n", i);
            printf(" - Tamaño: %" PRIu64 " bytes\n", s->validDataLength);
            printf(" - Primer cluster: %u\n", s->firstCluster);
        }
    }

    free(clusterBuf);
    close(fd);
    return 0;
}