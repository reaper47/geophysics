package arrayutils

import (
	"strings"
	"log"
)

func FindMaxNbValsFloat64(data [][]float64) int {
	max := len(data[0])
	for i := range data {
		lenArr := len(data[i])
		if lenArr > max {
			max = lenArr
		}
	}
	return max
}

func GatherItemsSameIndex(item float64, data []float64) []int {
	var indexes []int
	for key, val := range data {
		if val == item {
			indexes = append(indexes, key)
		}
	}
	return indexes
}

func GetColumnData(data [][]string, i int) []string {
	var colData []string
	for _, val := range data {
		colData = append(colData, val[i])
	}
	return colData[1:]
}

func GetTotalNumberEls2D(data [][]float64) int {
	els := 0
	for i, n := 0, len(data); i < n; i++ {
		for j, m := 0, len(data[i]); j < m; j++ {
			els++
		}
	}
	return els
}

func InvertArrayStr(data [][]string) [][]string {
    lenData := len(data)
    lenRow := len(data[0])
    log.Println(lenData)
    log.Println(lenRow)
    
    
    var arr [][]string
    for i := 0; i < lenRow; i++ {
        arr = append(arr, make([]string, 0))
        for j := 0; j < lenData; j++ {
            arr[i] = append(arr[i], data[j][i])
            log.Println(arr)
        }
    }
    return arr
}

func ResizeSlice(data []string, expand int) []string {
	if (len(data) + expand) > cap(data) {
		newSlice := make([]string, len(data), cap(data) + expand)
		copy(newSlice, data)
		return newSlice
	}
	return data
}

func RemoveElementFromSlice(slice []float64, i int) []float64 {
	return append(slice[:i], slice[i+1:]...)
}

func StringContainedInSlice(a string, arr []string) bool {
	for i := range arr {
		if strings.Contains(arr[i], a) {
			return true
		}
	}
	return false
}

