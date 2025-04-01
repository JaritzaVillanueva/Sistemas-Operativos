#!/bin/bash

#Verificacion de existencia del archivo
if [ ! -f "Lista1.txt" ]; then
	echo "Error: No se encontro el archivo"
	exit 1
fi

#Leer el archivo  linea por linea
while IFS= read -r linea #-r: evita \ como especial
do
	#extrae DNI
	dni=${linea%%	*} #todo hasta el primer espacio
	#extra nombre completo
	resto=${linea#*	} #quitamos DNI

	#separacion 
	apellido=${resto%%,*} #todo antes de la coma
	nombre=${resto#*, } #todo despues de la coma

	#imprimir los datos extraidos
	printf "DNI: $dni - NOMBRE: $nombre - APELLIDO: $apellido\n"
done < Lista1.txt
