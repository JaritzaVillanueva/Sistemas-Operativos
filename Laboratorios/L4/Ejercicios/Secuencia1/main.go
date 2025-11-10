// Ítem (c): Productor–Consumidor con protección completa de la sección crítica
// - Buffer circular de tamaño 5 (cola acotada).
// - Productor espera si buffer lleno (count == len(buffer)).
// - Consumidor espera si buffer vacío (count == 0).
// - Acceso protegido con Mutex; sincronización con Cond: notFull / notEmpty.
//
// Resultado: no hay lecturas de posiciones no producidas ni escrituras sobre posiciones no consumidas.

package main

import (
	"fmt"
	"sync"
)

var (
	buf      = make([]int, 5)    // buffer circular (los valores pueden ser cualquier int)
	head     = 0                 // índice de lectura (consume)
	tail     = 0                 // índice de escritura (produce)
	count    = 0                 // cuántos elementos hay en el buffer
	mu       sync.Mutex          // protege head, tail, count y buf
	notFull  = sync.NewCond(&mu) // condición: buffer NO lleno (productor puede producir)
	notEmpty = sync.NewCond(&mu) // condición: buffer NO vacío (consumidor puede consumir)
	wg       sync.WaitGroup
)

// produce: inserta un item; si el buffer está lleno, espera.
func produce(item int) {
	mu.Lock()
	for count == len(buf) { // buffer lleno → esperar
		notFull.Wait() // libera el mutex mientras duerme
	}
	buf[tail] = item // escribir en 'tail'
	fmt.Printf("productor -> puso %3d en [%d]  (head=%d, tail=%d, count=%d)\n",
		item, tail, head, tail, count)
	tail = (tail + 1) % len(buf) // avanzar cola
	count++                      // ahora hay un elemento más
	notEmpty.Signal()            // avisar a consumidor que ya hay datos
	mu.Unlock()
}

// consume: retira un item; si el buffer está vacío, espera.
func consume() int {
	mu.Lock()
	for count == 0 { // buffer vacío → esperar
		notEmpty.Wait() // libera el mutex mientras duerme
	}
	item := buf[head] // leer de 'head'
	fmt.Printf("consumidor -> leyó %3d de [%d]  (head=%d, tail=%d, count=%d)\n",
		item, head, head, tail, count)
	head = (head + 1) % len(buf) // avanzar cabeza
	count--                      // ahora hay un elemento menos
	notFull.Signal()             // avisar a productor que hay espacio
	mu.Unlock()
	return item
}

func productor() {
	defer wg.Done()
	for n := 0; n < 20; n++ { // mismo volumen que el ejemplo del enunciado
		item := n * n
		produce(item)
	}
}

func consumidor() {
	defer wg.Done()
	for n := 0; n < 20; n++ {
		_ = consume()
	}
}

func main() {
	wg.Add(2)
	go productor()
	go consumidor()
	wg.Wait()
}
