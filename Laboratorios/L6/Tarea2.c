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