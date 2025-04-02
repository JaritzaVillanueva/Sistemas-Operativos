#!/bin/bash

#Verificacion de existencia del archivo
if [ ! -f "data.txt" ]; then
	echo "ERROR: No existe el archivo"
	exit 1
fi

#Leer el archivo lina por linea
while IFS= read -r linea
do
	codigo=${linea%% *}
	resto=${linea#*   }

	resto="${resto,,}"
	#part1=${resto%%,*}
	#part2=${resto#*, }
	resto=($resto) #sin comillas
	resto="${resto[@]^}"
	echo "$resto $codigo"
done < data.txt

