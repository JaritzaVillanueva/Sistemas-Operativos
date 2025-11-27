#include <stdio.h>          // Para printf, perror
#include <stdlib.h>         // Para exit, atoi
#include <fcntl.h>          // Para open
#include <unistd.h>         // Para read, lseek, close
#include <stdint.h>         // Para uint32_t

#include "exFATstruct.h"     // <-- AJUSTA: aquí va tu .h con exFatBootSector

int main(int argc, char *argv[]) {                 // Inicio de main con argumentos

    if (argc != 2) {                               // Verificamos que haya 1 argumento (n)
        printf("Uso: %s <n_entradas>\n", argv[0]); // Mostramos cómo usar el programa
        exit(1);                                   // Terminamos con código de error
    }

    int n = atoi(argv[1]);                         // Convertimos el argumento a entero

    if (n <= 0) {                                  // Comprobamos que n sea positivo
        printf("El numero de entradas debe ser positivo.\n");
        exit(1);                                   // Terminamos si el valor no es válido
    }

    const char *imageName = "exFAT.img";           // Nombre de la imagen indicada en el lab

    int fd = open(imageName, O_RDONLY);            // Abrimos la imagen en solo lectura
    if (fd < 0) {                                  // Si open devuelve < 0, hubo error
        perror("No se pudo abrir exFAT.img");      // Mostramos mensaje de error del sistema
        exit(1);                                   // Terminamos
    }

    exFatBootSector boot;                          // Estructura para guardar el boot sector

    ssize_t leidos = read(fd, &boot, sizeof(boot));// Leemos tantos bytes como ocupa la struct
    if (leidos != sizeof(boot)) {                  // Si no leímos el tamaño completo
        perror("No se pudo leer el Boot Sector");  // Mostramos error
        close(fd);                                 // Cerramos el archivo
        exit(1);                                   // Terminamos
    }

    int bytesPerSector = 1 << boot.BytePerSector;  // Bytes por sector = 2^(BytePerSector)

    off_t fatStartByte =                           // Calculamos el inicio de la FAT en bytes
        (off_t)boot.FATOffset * bytesPerSector;    // FATOffset está en sectores

    int totalFatEntries =                          // Número total de entradas en la FAT
        (boot.FATlen * bytesPerSector) / 4;        // Cada entrada ocupa 4 bytes

    if (n > totalFatEntries) {                     // Si n es mayor que el total de entradas
        printf("Advertencia: n (%d) > entradas FAT (%d). "
               "Se usara n = %d.\n",
               n, totalFatEntries, totalFatEntries);
        n = totalFatEntries;                       // Ajustamos n al máximo posible
    }

    printf("Primeras %d entradas de la FAT:\n", n);// Encabezado informativo

    for (int i = 0; i < n; i++) {                  // Recorremos desde la entrada 0 a la n-1

        off_t entryOffset =                        // Calculamos offset de la entrada i
            fatStartByte + (off_t)i * 4;           // Cada entrada ocupa 4 bytes

        if (lseek(fd, entryOffset, SEEK_SET) < 0) {// Movemos puntero de archivo al offset
            perror("Error en lseek al leer la FAT");
            close(fd);                             // Cerramos archivo
            exit(1);                               // Terminamos
        }

        uint32_t value;                            // Variable para guardar la entrada FAT

        leidos = read(fd, &value, sizeof(value));  // Leemos 4 bytes de la FAT
        if (leidos != sizeof(value)) {             // Si no se leen 4 bytes exactos
            perror("Error al leer una entrada de la FAT");
            close(fd);                             // Cerramos archivo
            exit(1);                               // Terminamos
        }

        printf("Entrada %4d: 0x%08X\n",            // Imprimimos indice y valor en hex
               i, value);                          // %08X -> 8 dígitos hex con ceros delante
    }

    close(fd);                                     // Cerramos la imagen

    return 0;                                      // Fin normal del programa
}
