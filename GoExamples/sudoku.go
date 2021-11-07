package main

import (
	"sync"
)

func isSudokuValid(sudoku [9][9]int) bool {
	wg := new(sync.WaitGroup)
	goroutinesAmounts := 5 // 1 goroutine for rows, 1 for columns and 1 for every 3 squares
	results := make(chan bool, goroutinesAmounts)
	for i := 0; i < 3; i++ {
		wg.Add(1)
		go sudokuLineValidator(sudoku, i, wg, results)
	}
	wg.Add(1)
	go columnsValidator(sudoku, wg, results)
	wg.Add(1)
	go rowsValidator(sudoku, wg, results)
	go func() {
		wg.Wait()
		close(results)
	}()
	for val := range results {
		if !val {
			return false
		}
	}
	return true
}

func sudokuLineValidator(matrix [9][9]int, sudokuRowNum int, wg *sync.WaitGroup, resultChannel chan bool) {
	defer wg.Done()
	for i := sudokuRowNum * 3; i < (sudokuRowNum + 1)  * 3; i += 3 {
		for j := 0; j < len(matrix); j += 3 {
			squareDictionary := map[int]int{}
			for sqi := i; sqi < i + 3; sqi++ {
				for sqj := j; sqj < j + 3; sqj++ {
					squareDictionary[matrix[sqi][sqj]] += 1
				}
			}
			if !checkElementsDistinct(squareDictionary) {
				resultChannel <- false
				return
			}
		}
	}
	resultChannel <- true
}

func columnsValidator(matrix [9][9]int, wg *sync.WaitGroup, resultChannel chan bool) {
	defer wg.Done()
	columnDictionary := map[int]int{}
	for i := 0; i < len(matrix); i++ {
		for j := 0; j < len(matrix[i]); j++ {
			columnDictionary[matrix[j][i]] += 1
		}
		if !checkElementsDistinct(columnDictionary) {
			resultChannel <- false
			return
		}
		columnDictionary = map[int]int{}
	}
	resultChannel <- true
}

func rowsValidator(matrix [9][9]int, wg *sync.WaitGroup, resultChannel chan bool) {
	defer wg.Done()
	rowDictionary := map[int]int{}
	for i := 0; i < len(matrix); i++ {
		for j := 0; j < len(matrix[i]); j++ {
			rowDictionary[matrix[i][j]] += 1
		}
		if !checkElementsDistinct(rowDictionary) {
			resultChannel <- false
			return
		}
		rowDictionary = map[int]int{}
	}
	resultChannel <- true
}

func checkElementsDistinct(values map[int]int) bool {
	for key, value := range values {
		if key != 0 && value > 1 {
			return false
		}
	}
	return true
}
