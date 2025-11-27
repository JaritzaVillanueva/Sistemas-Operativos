/*
    Recibe como argumento un número que indica el número de un cluster.
    El programa debe devolver si está libre u ocupado.
    La búsqueda debe ser realizada en el mapa de bits.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "exFATstruct.h"

typedef struct
{
    unsigned char entryType; // Primer byte: tipo de entrada
    unsigned char rest[31];  // No lo necesitamos
} __attribute__((packed)) GenericDirEntry;

typedef struct
{
    unsigned char entryType; // Debe ser 0x81 para Allocation Bitmap
    unsigned char bitmapFlags;
    unsigned char reserved1[18];
    uint32_t firstCluster; // Primer cluster del archivo bitmap
    uint64_t dataLength;   // Tamaño del bitmap en bytes
} __attribute__((packed)) BitmapDirEntry;

// ----------------------------------------------------------------------

int main(int argc, char *argv[])
{

    // ------------------------------------------------------------------
    // Validación de parámetros
    // ------------------------------------------------------------------
    if (argc != 3)
    {
        printf("Uso: %s <imagen.exfat> <numero_cluster>\n", argv[0]);
        exit(1);
    }

    int clusterN = atoi(argv[2]); // cluster a consultar

    // Abrimos la imagen
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("No se pudo abrir la imagen");
        exit(1);
    }

    // ------------------------------------------------------------------
    // Leemos el boot sector
    // ------------------------------------------------------------------
    exFatBootSector boot;
    if (read(fd, &boot, sizeof(boot)) != sizeof(boot))
    {
        perror("No se pudo leer el boot sector");
        close(fd);
        exit(1);
    }

    // Calculamos datos esenciales del sistema exFAT
    int bytesPerSector = (int)pow(2, boot.BytePerSector);
    int sectorsPerCluster = (int)pow(2, boot.SectorPerCluster);
    int bytesPerCluster = bytesPerSector * sectorsPerCluster;

    // ------------------------------------------------------------------
    // Calculamos donde empieza el CLUSTER DEL DIRECTORIO RAIZ
    // ------------------------------------------------------------------

    int rootCluster = boot.RootDirFirstCluster; // ejemplo: suele ser 2
    int heapSector0 = boot.ClusterHeapOffset;   // primer sector de data

    int rootIndex = rootCluster - 2; // clusterN → índice 0-based
    int rootStartSector = heapSector0 + rootIndex * sectorsPerCluster;
    off_t rootStartByte = (off_t)rootStartSector * bytesPerSector;

    // Nos posicionamos en el cluster del root
    if (lseek(fd, rootStartByte, SEEK_SET) < 0)
    {
        perror("Error en lseek al root");
        close(fd);
        exit(1);
    }

    // Leemos el cluster completo del root
    unsigned char *rootBuf = malloc(bytesPerCluster);
    if (!rootBuf)
    {
        perror("Error malloc");
        close(fd);
        exit(1);
    }

    if (read(fd, rootBuf, bytesPerCluster) != bytesPerCluster)
    {
        perror("No se pudo leer root directory");
        free(rootBuf);
        close(fd);
        exit(1);
    }

    // ------------------------------------------------------------------
    // Buscamos la entrada tipo 0x81 (Allocation Bitmap)
    // ------------------------------------------------------------------

    uint32_t bitmapFirstCluster = 0;

    int entries = bytesPerCluster / 32;

    for (int i = 0; i < entries; i++)
    {
        GenericDirEntry *g = (GenericDirEntry *)(rootBuf + i * 32);

        if (g->entryType == 0x00)
            break; // fin de entradas válidas

        if (g->entryType == 0x81)
        {
            // reinterpretamos como BitmapDirEntry
            BitmapDirEntry *bm = (BitmapDirEntry *)(rootBuf + i * 32);
            bitmapFirstCluster = bm->firstCluster;
            break;
        }
    }

    free(rootBuf);

    if (bitmapFirstCluster == 0)
    {
        fprintf(stderr, "No se encontro entrada 0x81 (bitmap)\n");
        close(fd);
        exit(1);
    }

    // ------------------------------------------------------------------
    // Ahora localizamos el CLUSTER DEL BITMAP
    // ------------------------------------------------------------------

    int bmpIndex = bitmapFirstCluster - 2;
    int bmpStartSector = heapSector0 + bmpIndex * sectorsPerCluster;
    off_t bmpStartByte = (off_t)bmpStartSector * bytesPerSector;

    // ------------------------------------------------------------------
    // Convertimos clusterN a bit dentro del bitmap:
    //
    // bit 0 -> cluster 2
    // bit 1 -> cluster 3
    // ...
    // bitIndex = clusterN - 2
    // ------------------------------------------------------------------

    int bitIndex = clusterN - 2;

    int byteIndex = bitIndex / 8; // cual byte
    int bitInByte = bitIndex % 8; // cual bit dentro del byte

    // Posición del byte dentro del archivo
    off_t targetByteOffset = bmpStartByte + byteIndex;

    if (lseek(fd, targetByteOffset, SEEK_SET) < 0)
    {
        perror("Error lseek bitmap");
        close(fd);
        exit(1);
    }

    unsigned char bitmapByte;
    if (read(fd, &bitmapByte, 1) != 1)
    {
        perror("No se pudo leer byte del bitmap");
        close(fd);
        exit(1);
    }

    // Extraemos el bit
    int ocupado = (bitmapByte & (1 << bitInByte)) != 0;

    if (ocupado)
        printf("Cluster %d esta OCUPADO (bitmap)\n", clusterN);
    else
        printf("Cluster %d esta LIBRE (bitmap)\n", clusterN);

    close(fd);
    return 0;
}