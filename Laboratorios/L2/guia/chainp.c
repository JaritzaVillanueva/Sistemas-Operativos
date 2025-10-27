#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* Este programa crea una cadena de procesos. Es decir el padre  */
/* crea un hijo, este a su vez crea otro y asi en forma sucesiva */
/* Ejm 2.5 del libro UNIX Programacion Practica - Kay Robbins    */
/*                                    Steve Robbins              */
/* Modificado por Alejandro Bello Ruiz - Informática PUCP        */

int main(void){
    int i, status;
    pid_t child;       /* pid_t es un tipo definido en types.h */

    for (i=1; i<4; i++) 
        if((child=fork())) break;

    fprintf(stderr,"Esta es la vuelta Nro %d\n",i);
    fprintf(stderr,"Recibi de fork el valor de %d\n",child);
    fprintf(stderr,"Soy el proceso %d con padre %d\n\n",getpid(),getppid());

    sleep(30);
    wait(&status);
    return 0;
}