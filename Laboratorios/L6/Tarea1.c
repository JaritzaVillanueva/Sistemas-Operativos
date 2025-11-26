/*
    Recibe como argumento un número que indica el número de un cluster. 
    El programa debe devolver si está libre u ocupado. 
    La búsqueda debe ser realizada en el área de la FAT.
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

int main(int argc, char *argv[]){
    int fd, exp;
    exFatBootSector boot;

    // 1. Validamos que el usuario dé 2 argumentos
    if(argc!=3){
        printf("Uso: %s <Image File System>\n", argv[0]);
        exit(1);
    }

    // 2. Convertimos el numero de cluster)
    int clusterN = atoi(argv[2]);

    // 3. Abrir la imagen del disco
    if((fd = open(argv[1], O_RDONLY)) < 0)
        perror("No se pudo abrir imagen del disco\n");

    // 4. Leer el sector de boot 
    if(read(fd, &boot, sizeof(boot)) < 0)
        perror("No se pudo leer imagen del disco\n");

    // 5. Calculamos bytes por sector y sectores por cluster
    int bytesPerSector = pow(2, boot.BytePerSector); // numero de bytes por sector

    // 6. Calculamos en que byte inicia FAT
    off_t FAT_start_byte = (off_t)boot.FATOffset * bytesPerSector;

    // 7. Cada entrada de FAT ocupa 4 bytes
    off_t FAT_entry_offset = FAT_start_byte + (clusterN * 4);

    // 8. Nos movemos a esa entrada
    if (lseek(fd, FAT_entry_offset, SEEK_SET) < 0)
        perror("No se pudo mover el cursor a la entrada de FAT\n");
    
    // 9. Leemos los 4 bytes de la entrada FAT
    uint32_t FATvalue;
    if (read(fd, &FATvalue, 4) != 4)
        perror("No se pudo leer la entrada de FAT\n");
    
    // 10. Interpretamos el valor
    if (FATvalue == 0x00000000) {
        printf("El cluster %d está LIBRE.\n", clusterN);
    } else {
        printf("El cluster %d está OCUPADO.\n", clusterN);
    }

    close(fd);
    exit(0);
}