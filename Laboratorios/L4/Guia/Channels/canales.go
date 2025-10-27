// Tomado de https://go.dev/tour/concurrency/2
package main

import "fmt"

func sum(s []int, c chan int) {
	sum := 0
	for _, v := range s {
		sum += v
	}
	c <- sum // enviar la suma al canal c
}

func main() {
	s := []int{7, 2, 8, -9, 4, 0}

	c := make(chan int)            // canal no bufferizado
	go sum(s[:len(s)/2], c)        // goroutine 1: suma la primera mitad
	go sum(s[len(s)/2:], c)        // goroutine 2: suma la segunda mitad

	x, y := <-c, <-c               // recibir dos resultados (orden no garantizado)
	fmt.Println(x, y, x+y)
}
