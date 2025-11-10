package main

import (
	"fmt"
	"sync"
)

func routine(n int) {
	defer wg.Done() // Indica que la goroutine ha terminado - resta 1 al contador del WaitGroup
	fmt.Printf("I'm goroutine %d\n", n)
}

var wg sync.WaitGroup

func main() {
	for i := 0; i < 5; i++ {
		wg.Add(1) // Incrementa el contador del WaitGroup
		go routine(i)
	}
	wg.Wait() // Espera a que todas las goroutines terminen
}
