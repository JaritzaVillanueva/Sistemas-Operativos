package main

import (
	"fmt"
	"os"
)

func main() {
	var n int
	nArg := len(os.Args)
	if nArg != 2 {
		fmt.Printf("Usage %v <number>\n", os.Args[0])
		os.Exit(1)
	}

	fmt.Sscan(os.Args[1], &n) // Sscan(cadena, &variable1, &variable2, ...)
	fmt.Println(factorial(n))
	fmt.Println(libof(n))
}

// Función recursiva: factorial
func factorial(n int) int {
	if n == 0 {
		return 1
	}
	return n * factorial(n-1)
}

// Función que genera una lista de números de Fibonacci
func libof(n int) []int {
	slice := make([]int, 0) //make([]tipo, long) - crea un slice vacío
	f := fibonacci()
	for i := 0; i < n; i++ {
		// append(sliceExistente, elementos...) - agrega elementos al slice
		slice = append(slice, f()) 
	}
	return slice
}

// Función que devuelve otra función (closure)
func fibonacci() func() int {
	var t int
	f1 := 0
	f2 := 1
	/* La función anónima devuelve el siguiente número de Fibonacci */
	return func() int {
		t = f1
		f1 = f2
		f2 = t + f2
		return t
	}
}