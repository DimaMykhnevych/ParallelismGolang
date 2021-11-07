package main

import (
	"fmt"
	"math"
	"math/rand"
	"sync"
	"time"
)

func main() {
	// Example 1
	//rows := 100
	//columns := 1000000
	//a := createAndFillMatrix(rows, columns)
	//
	////single calculation
	//start := time.Now()
	//findMaxValue(a)
	//duration := time.Since(start)
	//fmt.Println(duration)
	////multiple calculation

	//var timeArray[] time.Duration
	//for i := 0; i < 1000; i++ {
	//	start := time.Now()
	//	findMaxValue(a)
	//	duration := time.Since(start)
	//	timeArray = append(timeArray, duration)
	//}
	//var min time.Duration
	//for i, e := range timeArray {
	//	if i==0 || e < min {
	//		min = e
	//	}
	//}
	//fmt.Println(min)

	//Example 2
	validSudoku := [9][9]int{
		{1, 4, 7, 0, 0, 0, 0, 0, 3},
		{2, 5, 0, 0, 0, 1, 0, 0, 0},
		{3, 0, 9, 0, 0, 0, 0, 0, 0},
		{0, 8, 0, 0, 2, 0, 0, 0, 4},
		{0, 0, 0, 4, 1, 0, 0, 2, 0},
		{9, 0, 0, 0, 0, 0, 6, 0, 0},
		{0, 0, 3, 0, 0, 0, 0, 0, 9},
		{4, 0, 0, 0, 0, 2, 0, 0, 0},
		{0, 0, 1, 0, 0, 8, 0, 0, 7},
	}
	start := time.Now()
	isValid := isSudokuValid(validSudoku)
	duration := time.Since(start)
	fmt.Println("Is Valid:", isValid)
	fmt.Println(duration.Nanoseconds())
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
