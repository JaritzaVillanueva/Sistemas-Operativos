// Tomado de https://go.dev/tour/concurrency/1
package main

import (
	"fmt"
	"sync"
	"time"
)

func say(s string) {
	for i := 0; i < 5; i++ {
		time.Sleep(100 * time.Millisecond)
		fmt.Println(s)
	}
}

func sayAsync(s string, wg *sync.WaitGroup) {
	defer wg.Done()
	say(s)
}

func main() {
	var wg sync.WaitGroup
	wg.Add(1)                 // indica que vamos a esperar a una goroutine
	go sayAsync("world", &wg) // lanza una goroutine
	say("hello")              // también podría ser una goroutine y hacer wg.Add(1)
	wg.Wait()                 // espera a que terminen las goroutines
}

/******* OTRO EJERCICIO *******/
/*
func worker(id int, wg *sync.WaitGroup) {
	defer wg.Done() // avisa que esta goroutine terminó
	fmt.Printf("Trabajador %d: iniciando\n", id)
	time.Sleep(time.Second) // simula trabajo
	fmt.Printf("Trabajador %d: terminado\n", id)
}

func main() {
	var wg sync.WaitGroup // crea un WaitGroup, para esperar a varias goroutines

	// Queremos lanzar 3 goroutines
	for i := 1; i <= 3; i++ {
		wg.Add(1)         // registramos una tarea pendiente
		go worker(i, &wg) // lanzamos la goroutine
	}

	wg.Wait() // bloquea hasta que todos los Done() se ejecuten
	fmt.Println("Todos los trabajadores han terminado ✅")
}
*/
