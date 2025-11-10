package main

import "fmt"

func main() {
	/* Usando un channel para sincronizar goroutines */
	done := make(chan struct{}) //canal de señalización vacío
	for x := 0; x < 5; x++ {
		go func(count int) {
			fmt.Printf("I'm goroutine %d\n", count)
			done <- struct{}{} // Enviamos una señal de que la goroutine ha terminado
		}(x) // Pasamos x como copia parámetro para evitar condiciones de carrera
	}
	/* Esperamos a que todas las goroutines terminen */
	for x := 0; x < 5; x++ {
		<-done // Recibimos la señal de que una goroutine ha terminado
	}
}
