package main

import "fmt"

func fibonacci(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < 2*n; i++ {
		c <- x
		fmt.Printf("Orden:%v - Valor:%v\n", i, x)
		x, y = y, x+y
	}
	close(c) // cerrar el canal cuando ya no se enviarán más datos
}

func main() {
	c := make(chan int, 100) // canal con buffer de 100 posiciones
	fmt.Println(cap(c))      // imprime la capacidad del canal
	go fibonacci(cap(c), c)  // ejecuta fibonacci en una goroutine
	for i := range c {       // lee los valores hasta que el canal se cierre
		fmt.Println(i)
	}
}
