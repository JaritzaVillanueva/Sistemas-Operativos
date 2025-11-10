package main

import (
	"fmt"
	"sync"
)

// Declaración de variables globales
var buffer []int                // Buffer compartido entre productores y consumidores
var mutex sync.Mutex            // Mutex para proteger el acceso concurrente al buffer
var wg sync.WaitGroup           // WaitGroup para esperar que todas las goroutines terminen
var cond = sync.NewCond(&mutex) // Condición asociada al mutex para coordinación

// Función produce: agrega un item al buffer
func produce(item int) {
	mutex.Lock()            // Bloquea el acceso al buffer para evitar condiciones de carrera
	for len(buffer) >= 10 { // Si el buffer tiene 10 o más elementos, espera
		cond.Wait() // Espera hasta que haya espacio disponible
	}
	buffer = append(buffer, item) // Agrega el nuevo item al buffer
	cond.Signal()                 // Señala a otras goroutines (como consumidores) que hay un cambio
	mutex.Unlock()                // Libera el mutex
	wg.Done()                     // Indica al WaitGroup que esta goroutine ha terminado
}

// Función consume: retira un item del buffer
func consume() {
	mutex.Lock()           // Bloquea el acceso al buffer
	for len(buffer) == 0 { // Si el buffer está vacío, espera
		cond.Wait() // Espera hasta que haya al menos un elemento
	}
	item := buffer[0]                     // Toma el primer elemento del buffer
	buffer = buffer[1:]                   // Elimina el elemento consumido del buffer
	cond.Signal()                         // Señala a los productores que hay espacio disponible
	mutex.Unlock()                        // Libera el mutex
	fmt.Println("Valor consumido:", item) // Imprime el valor consumido
	wg.Done()                             // Indica al WaitGroup que esta goroutine ha terminado
}

// Función principal
func main() {
	wg.Add(4) // Indica que habrá 4 goroutines (2 productores y 2 consumidores)

	go produce(1) // Lanza una goroutine para producir el item 1
	go produce(2) // Lanza otra goroutine para producir el item 2
	go consume()  // Lanza una goroutine para consumir un item
	go consume()  // Lanza otra goroutine para consumir un item

	wg.Wait() // Espera a que todas las goroutines terminen antes de salir
}
