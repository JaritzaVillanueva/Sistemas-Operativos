#!/bin/bash

#funcion  verificacion primo
es_primo(){
	local num=$1
	local i
	if [  $num -le 1 ]; then
		return 1
	fi # no es primo
	for ((i=2; i*i<=num; i++))
	do
		if [ $(( num % i )) -eq 0 ]; then
			return  1 #no es primo
		fi
	done
	return 0;
}

#Ingreso de input
read -p "Ingresa numero 1: " num1
read -p "Ingresa numero 2: " num2

#Verificacion del mayor numero
if [ $num1 -ge $num2 ]; then
	mayor=$num1
	menor=$num2
else
	mayor=$num2
	menor=$num1
fi

#iteracion
#set -x
for((i=menor; i<=mayor; i++))
do
	printf "$i "
	if es_primo $i; then
		printf "(primo)"
	fi
	printf "\n"
done
#set  +x
