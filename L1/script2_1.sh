#!/bin/bash

#Funcion: capitalizar palabra
capitalizar(){
	local primeraVez=0
	while IFS= read -r -n1 car
	do
		if [[ $primeraVez -eq 0 ]]; then
			car=$(echo "$car" | tr '[:lower:]' '[:upper:]')
			primeraVez=1
		elif [[ $car == " " ]]; then
			primeraVez=0
		else
			car=$(echo "$car" | tr '[:upper:]' '[:lower:]')
		fi
		palabra+=$car
	done <<< $1
	echo "$palabra"
} 

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
	nombre="${nombre,,}"
	nombre=$(capitalizar "$nombre")
	#nombrePunto="${nombre// /.}"
	primerNombre=${nombre% *}
	primerApellido=${apellido%% *}
	#impresion de las lineas
	echo "$dni:${apellido^^},$nombre:${primerNombre,,}.${primerApellido,,}@pucp.edu.pe" >> Lista2.txt
done < Lista1.txt
