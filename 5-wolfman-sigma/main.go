package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

// ReadMatrixFromFile reads the matrix from a file
func ReadMatrixFromFile(filename string) ([][]float64, [][]float64, int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, 0, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	n, _ := strconv.Atoi(scanner.Text())

	matrix1 := make([][]float64, n)
	matrix2 := make([][]float64, n)

	// Reading the first matrix
	for i := 0; i < n; i++ {
		scanner.Scan()
		line := strings.TrimSpace(scanner.Text())
		elements := strings.Split(line, "  ")
		matrix1[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			matrix1[i][j], _ = strconv.ParseFloat(elements[j], 64)
		}
	}

	// Reading the second matrix
	for i := 0; i < n; i++ {
		scanner.Scan()
		line := strings.TrimSpace(scanner.Text())
		elements := strings.Split(line, "  ")
		matrix2[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			matrix2[i][j], _ = strconv.ParseFloat(elements[j], 64)
		}
	}

	return matrix1, matrix2, n, nil
}

// SerialMatrixMultiplication performs matrix multiplication serially
func SerialMatrixMultiplication(matrix1, matrix2 [][]float64, n int) [][]float64 {
	result := make([][]float64, n)
	for i := 0; i < n; i++ {
		result[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			for k := 0; k < n; k++ {
				result[i][j] += matrix1[i][k] * matrix2[k][j]
			}
		}
	}
	return result
}

// ParallelMatrixMultiplication performs matrix multiplication in parallel using Goroutines
func ParallelMatrixMultiplication(matrix1, matrix2 [][]float64, n int) [][]float64 {
	result := make([][]float64, n)
	for i := range result {
		result[i] = make([]float64, n)
	}

	var wg sync.WaitGroup
	// Split the work into Goroutines
	for i := 0; i < n; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			for j := 0; j < n; j++ {
				for k := 0; k < n; k++ {
					result[i][j] += matrix1[i][k] * matrix2[k][j]
				}
			}
		}(i)
	}

	// Wait for all Goroutines to finish
	wg.Wait()
	return result
}

// CompareMatrices compares two matrices and returns true if they are the same
func CompareMatrices(matrix1, matrix2 [][]float64, n int) bool {
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			if matrix1[i][j] != matrix2[i][j] {
				return false
			}
		}
	}
	return true
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <input file>")
		return
	}

	filename := os.Args[1]

	// Read matrices from file
	matrix1, matrix2, n, err := ReadMatrixFromFile(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	// Serial Execution
	start := time.Now()
	resultSerial := SerialMatrixMultiplication(matrix1, matrix2, n)
	serialDuration := time.Since(start)
	fmt.Printf("Serial execution time: %.6f seconds\n", serialDuration.Seconds())

	// Parallel Execution
	start = time.Now()
	resultParallel := ParallelMatrixMultiplication(matrix1, matrix2, n)
	parallelDuration := time.Since(start)
	fmt.Printf("Parallel execution time: %.6f seconds\n", parallelDuration.Seconds())

	// Verify that the results are the same
	if CompareMatrices(resultSerial, resultParallel, n) {
		fmt.Println("Results match: true")
	} else {
		fmt.Println("Results match: false")
	}
}
