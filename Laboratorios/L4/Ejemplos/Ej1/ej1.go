package main

import (
	"fmt"
	"time"
)

func routine(n int) {
	fmt.Printf("I'm goroutine %d\n", n)
}

func main() {
	for i := 0; i < 10; i++ {
		go routine(i)
	}
	time.Sleep(1 * time.Second) // Espera para que las goroutines terminen - 1 segundo
}
