#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "exFATstruct.h"

int main(int n, char *name[]){
    int fd, exp;
    exFatBootSector boot;

    // Validar argumentos de entrada: nombre del programa y nombre de la imagen
    if(n!=2){
        printf("Uso: %s <Image File System>\n", name[0]);
        exit(1);
    }

    // Abrir la imagen del disco
    if((fd = open(name[1], O_RDONLY)) < 0)
        perror("No se pudo abrir imagen del disco\n");

    // Leer el sector de boot 
    if(read(fd, &boot, sizeof(boot)) < 0)
        perror("No se pudo leer imagen del disco\n");

    printf("\n\tDatos relevantes del disco con sistema de archivo exFat\n");
    printf("El tamaño de la imagen en sectores es de: %ld\n", boot.sizeVol);
    printf("La FAT se encuentra en el sector: %d\n", boot.FATOffset);
    printf("El tamaño de la FAT en sectores es de: %d\n", boot.FATlen);
    printf("La data inicia en el sector: %d\n", boot.ClusterHeapOffset);
    printf("La cantidad de espacio para la data en clusters es: %d\n", boot.ClusterCount);
    printf("El directorio raíz se encuentra en el cluster:%d\n", boot.RootDirFirstCluster);
    printf("El tamaño de un sector en bytes es de:%d\n", boot.BytePerSector);
    exp = boot.SectorPerCluster;
    printf("El número de sectores por cluster es de:%d\n", (int)pow(2.0, exp));
    printf("Número de FAT´s en este sistema de archivos:%d\n", boot.NumberFats);

    exit(0);
}