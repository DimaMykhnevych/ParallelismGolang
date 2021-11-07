package main

import (
	"fmt"
	"math"
	"math/rand"
	"sync"
	"time"
)

func main() {
	rows := 100
	columns := 100000
	a := createAndFillMatrix(rows, columns)
	start := time.Now()
	result := findMaxValue(a)
	duration := time.Since(start)
	fmt.Println(result)
	fmt.Println(duration)
}

func findMaxValue(matrix [][]int) int {
	rowsAmount := len(matrix)
	wg := new(sync.WaitGroup)
	results := make(chan int, rowsAmount)
	for i := 0; i < rowsAmount; i++ {
		wg.Add(1)
		go matrixSearcher(matrix[i], wg, results)
	}
	go func() {
		wg.Wait()
		close(results)
	}()
	max := math.MinInt32
	for val := range results {
		if val > max {
			max = val
		}
	}
	return max
}

func matrixSearcher(row []int, wg *sync.WaitGroup, resultChannel chan int) {
	defer wg.Done()
	max := math.MinInt32
	for i := range row {
		if row[i] > max {
			max = row[i]
		}
	}
	resultChannel <- max
}

func createAndFillMatrix(rows, columns int) [][]int {
	a := make([][]int, rows)
	for i := range a {
		a[i] = make([]int, columns)
	}

	for i := 0; i < rows; i++ {
		for j := 0; j < columns; j++ {
			a[i][j] = rand.Intn(1000001)
		}
	}
	return a
}
