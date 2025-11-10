package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

func main () {
	// Estructuras
	var v Vertex
	v.X = 4
	v.Y = 6

	fmt.Println(v)
	fmt.Println(v.X)
	fmt.Println(v.Y)

	w := Vertex{1, 2}

	fmt.Println(w)
	fmt.Println(w.X)
	fmt.Println(w.Y)

	//Arreglos
	var (
		a [3]int
		b [4]int	= [4]int{6,7,8,9}
		c [3]Vertex = [3]Vertex{{1,2}, {3,4}, {5,6}}
	)

	a[1] = 1
	a[2] = 2
	fmt.Println("Arreglos:")
	fmt.Println(a[0], a[1], a[2]) // 0 1 2
	fmt.Println(b[0], b[2])
	fmt.Println(c[0].X, c[1].X, c[2].X)

	d := [3]int{1,2,3}
	fmt.Println(d[1])

	// Slices
	primos := [6]int{2,3,5,7,11,13}

	slice1 := primos[1:4] // arreglo[inicio:fin] (incluye inicio, excluye fin)
	fmt.Println("Slices:", slice1) // Slices: [3 5 7]

	slice2 := make([]int, 2, 4) //make([]tipo, longitud, capacidad)
	fmt.Println("Slice2:", slice2, slice2[1], len(slice2), cap(slice2)) // Slice2: [0 0] 0 2 4
	slice2 = append(slice2, 1, 1) // append(sliceExistente, elementos...)
	fmt.Println("Slice2:", slice2, slice2[1], len(slice2), cap(slice2)) // Slice2: [0 0 1 1] 0 4 4

	// maps, siccionarios, hash, table

	d1 := make(map[string]int) //map[tipoLlave]tipoValor
	d1["uno"] = 1
	d1["dos"] = 2
	fmt.Println("Map:", d1) // Map: map[dos:2 uno:1]
}