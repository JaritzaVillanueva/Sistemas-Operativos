package main

import (
	"fmt"
	"math"
	"time"
)

func main() {
	// Uso de la sentencia defer
	defer fmt.Println("Fin de esta parte") // Se ejecuta al final de la función main

	// Uso del lazo for y la sentencia if
	for i := 0; i < 10; i++ {
		fmt.Println(i)
	}

	j := 10
	for j < 20 {
		fmt.Println(j)
		j++
	}

	arreglo := [6]int{20, 21, 22, 23, 24, 25}
	for _, valor := range arreglo { // Ignorar el índice con _
		fmt.Println(valor)
	}

	var v float64
	x := 0
	for {
		if v = math.Pow(2, float64(x)); v > 1024 { // calcula, asigna y evalúa
			break
		}
		fmt.Println("2 ^", x, "=", v)
		x++
	}

	// Uso de la sentencia switch
	ndia := time.Now().Weekday() // Sunday = 0, Monday = 1, ..., Saturday = 6
	hoy := int(ndia) // Convertir a entero

	switch hoy {
	case 0:
		fmt.Println("Hoy es domingo")
	case 1:
		fmt.Println("Hoy es lunes")
	case 2:
		fmt.Println("Hoy es martes")
	case 3:
		fmt.Println("Hoy es miércoles")
	case 4:
		fmt.Println("Hoy es jueves")
	case 5:
		fmt.Println("Hoy es viernes")
	case 6:
		fmt.Println("Hoy es sábado")
	}
}