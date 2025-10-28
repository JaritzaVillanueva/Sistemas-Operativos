package main

import "fmt"

func main() {
	naturals := make(chan int) // Channel para números naturales
	squares := make(chan int)  // Channel para cuadrados de números

	//Counter
	go func() {
		for x := 0; x < 20; x++ {
			// x se almacena en el channel naturals y se bloquea hasta que alguien lo reciba
			naturals <- x
		}
		close(naturals) // Cerramos el channel después de enviar los números
	}()

	//Squarer
	go func() {
		// x recive el valor del channel naturals y se bloquea hasta que haya un valor disponible
		for x := range naturals {
			squares <- x * x
		}
		close(squares)
	}()

	//Printer (in main goroutine)
	for x := range squares {
		fmt.Println(x)
	}
}
