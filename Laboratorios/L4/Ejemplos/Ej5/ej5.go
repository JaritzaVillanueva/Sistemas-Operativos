package main

import (
	"fmt"
	"sync"
)

func worker() {
	for x := 0; x < 1000000; x++ {
		count++ // Incrementamos la variable compartida
	}
	wg.Done() // Señalamos que esta goroutine ha terminado
}

var (
	count int
	wg    sync.WaitGroup
)

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)   // Incrementamos el contador del WaitGroup
		go worker() // Iniciamos la goroutine
	}
	wg.Wait() // Esperamos a que todas las goroutines terminen
	fmt.Printf("El valor esperado de count es: 5000000 y el valor final es %d\n", count)
}
