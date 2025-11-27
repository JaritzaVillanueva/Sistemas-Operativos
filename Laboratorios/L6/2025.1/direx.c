#include <stdio.h>          // printf, perror
#include <stdlib.h>         // exit, atoi
#include <fcntl.h>          // open
#include <unistd.h>         // read, lseek, close
#include <stdint.h>         // uint32_t

#include "exFATstruct.h"     // <-- AJUSTA este nombre a tu header real

int main(int argc, char *argv[]) {
    // Verificamos que recibimos exactamente 2 argumentos:
    // 1) nombre de la imagen
    // 2) número de entradas a mostrar
    if (argc != 3) {
        printf("Uso: %s <imagen_exfat> <n_entradas>\n", argv[0]);
        exit(1);
    }

    const char *imageName = argv[1];   // Nombre del archivo de imagen
    int n = atoi(argv[2]);             // Número de entradas a analizar

    // Validamos que n sea positivo
    if (n <= 0) {
        printf("El numero de entradas debe ser positivo.\n");
        exit(1);
    }

    // Abrimos la imagen exFAT en modo solo lectura
    int fd = open(imageName, O_RDONLY);
    if (fd < 0) {
        perror("No se pudo abrir la imagen");
        exit(1);
    }

    // Leemos el boot sector
    exFatBootSector boot;
    ssize_t leidos = read(fd, &boot, sizeof(boot));
    if (leidos != sizeof(boot)) {
        perror("No se pudo leer el Boot Sector");
        close(fd);
        exit(1);
    }

    // Calculamos bytes por sector = 2^(BytePerSector)
    int bytesPerSector = 1 << boot.BytePerSector;

    // Calculamos el byte donde inicia la FAT
    off_t fatStartByte = (off_t)boot.FATOffset * bytesPerSector;

    // Calculamos el total de entradas en la FAT
    int totalFatEntries = (boot.FATlen * bytesPerSector) / 4;

    // Ajustamos n si excede el número total de entradas
    if (n > totalFatEntries) {
        printf("Advertencia: n (%d) > total de entradas FAT (%d). Usando n = %d.\n",
               n, totalFatEntries, totalFatEntries);
        n = totalFatEntries;
    }

    printf("Analisis de las primeras %d entradas de la FAT:\n", n);

    // Recorremos las primeras n entradas
    for (int i = 0; i < n; i++) {
        // Offset de la entrada i en la FAT
        off_t entryOffset = fatStartByte + (off_t)i * 4;

        // Movemos el puntero del archivo a esa entrada
        if (lseek(fd, entryOffset, SEEK_SET) < 0) {
            perror("Error en lseek al leer la FAT");
            close(fd);
            exit(1);
        }

        // Leemos 4 bytes (uint32_t) de la FAT
        uint32_t value;
        leidos = read(fd, &value, sizeof(value));
        if (leidos != sizeof(value)) {
            perror("Error al leer una entrada de la FAT");
            close(fd);
            exit(1);
        }

        // Clasificamos el valor de la entrada
        const char *tipo;

        if (value == 0x00000000) {
            tipo = "LIBRE";
        } else if (value >= 0xFFFFFFF8) {
            tipo = "FIN DE CADENA / RESERVADO";
        } else if (value >= 0x00000002) {
            tipo = "SIGUIENTE CLUSTER";
        } else {
            // 0x00000001 o valores raros: poco usual, pero lo marcamos
            tipo = "VALOR ESPECIAL / RESERVADO";
        }

        // Imprimimos índice, valor y significado
        printf("Entrada %4d: 0x%08X -> %s\n", i, value, tipo);
    }

    // Cerramos la imagen
    close(fd);

    // Fin correcto
    return 0;
}
