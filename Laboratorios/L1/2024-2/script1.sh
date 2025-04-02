#!/bin/bash

numeros(){
	#Verificar que se ingresa 3 argumentos
	if [[ $# != 3 ]]; then
		echo "No se ingreso los 3 argumentos"
		exit 1
	fi

	#variables
	inicio=$1
	fin=$2
	valor=$3

	#iteracion entre el rango
	for((i=$inicio; i<=$fin; i++))
	do
		num=$i
		contador=0
		# Verificacion de cada digito
		while [[ $num -gt 0 ]]
		do
			digito=$(($num % 10))
			if [[ "$digito" == "$valor" ]]; then
				((contador++))
			fi
			num=$(($num/10))
		done
		
		#Verifica si tiene el valor dentro del numero
		if [[ $contador != 0 ]]; then
			echo "$i"
		fi
	done
}

numeros $1 $2 $3
