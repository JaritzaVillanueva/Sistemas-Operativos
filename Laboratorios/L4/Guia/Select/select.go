package main

import (
	"fmt"
	"time"
)

func main() {
	c1 := make(chan string) // canal sin buffer 1
	c2 := make(chan string) // canal sin buffer 2

	// goroutine 1: envía un mensaje cada 2 segundos
	go func() {
		for {
			c1 <- "from 1" // envía mensaje al canal c1
			time.Sleep(time.Second * 2) // espera 2 segundos
		}
	}()

	// goroutine 2: envía un mensaje cada 3 segundos
	go func() {
		for {
			c2 <- "from 2" // envía mensaje al canal c2
			time.Sleep(time.Second * 3) // espera 3 segundos
		}
	}()

	// goroutine 3: escucha ambos canales
	go func() {
		for {
			select {
			case msg1 := <-c1: // escucha mensaje del canal c1
				fmt.Println(msg1) // imprime el mensaje recibido
			case msg2 := <-c2: // escucha mensaje del canal c2
				fmt.Println(msg2) // imprime el mensaje recibido
			}
		}
	}()

	// evitar que main termine antes (espera una entrada)
	var input string
	fmt.Scanln(&input) // espera una entrada del usuario
}