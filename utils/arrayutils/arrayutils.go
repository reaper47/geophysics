package arrayutils

import (
    "strings"
    "errors"
    "geophysics/constants"
)

func FindMaxNbValsFloat64(data [][]float64) int {
    max := len(data[0])
    for _, arr := range data {
        lenArr := len(arr)
        if lenArr > max {
            max = lenArr
        }
    }
    return max
}

func GatherSameItemsIndex64(item float64, data []float64) []int {
    var indexes []int
    for key, val := range data {
        if val == item {
            indexes = append(indexes, key)
        }
    }
    return indexes
}

func GetColData(data [][]string, i int) []string {
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

func AddTwoArrays(a []float64, b []float64) ([]float64, error) {
    if len(a) != len(b) {
        return nil, errors.New(constants.ArraysNotSameLength)
    }
    
    var c []float64
    for i, n := 0, len(a); i < n; i++ {
        c = append(c, a[i] + b[i])
    }
    
    return c, nil
}

func AddArrWithConstant(data []float64, constant float64) []float64 {
    var results []float64
    for _, val := range data {
        results = append(results, val + constant)
    }
    return results
}

func DivideArrByConstant(data []float64, constant float64) []float64 {
    var results []float64
    for _, val := range data {
        results = append(results, val / constant)
    }
    return results
}

func MultiplyArrByConstant(data []float64, constant float64) []float64 {
    var results []float64
    for _, val := range data {
        results = append(results, val * constant)
    }
    return results
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
    for _, b := range arr {
        if strings.Contains(b, a) {
            return true
        }
    }
    return false
}

func SubtractArrayOneIndexAhead(data []float64, add []float64, appendZero bool) ([]float64, error) {
    if len(data) != len(add) {
        return nil, errors.New(constants.ArraysNotSameLength)
    }
    
    var values []float64
    if appendZero {
        values = append(values, 0.0)
    }
    
    for i, n := 1, len(data); i < n; i++ {
        values = append(values, data[i-1] - data[i] + add[i])
    }
    
    return values, nil
}

func SubtractSameArrayOneIndexAhead(data []float64) []float64 {
    var values []float64
    values = append(values, 0.0)
    
    for i, n := 1, len(data); i < n; i++ {
        values = append(values, values[i-1] - data[i])
    }
    
    return values
}

func SubtractTwoArrays(a []float64, b []float64) ([]float64, error) {
    if len(a) != len(b) {
        return nil, errors.New(constants.ArraysNotSameLength)
    }
    
    var c []float64
    for i, n := 0, len(a); i < n; i++ {
        c = append(c, a[i] - b[i])
    }
    
    return c, nil
}

func SubtractTwoArraysOneIndexAhead(a []float64, b []float64) ([]float64, error) {
    if len(a) != len(b) {
        return nil, errors.New(constants.ArraysNotSameLength)
    }
    
    var values []float64
    values = append(values, 0.0)
    
    for i, n := 1, len(a); i < n; i++ {
        values = append(values, b[i] - a[i-1])
    }
    return values, nil
}

func SubtractValueFromArray(value float64, data []float64) []float64 {
    var values []float64
    for _, val := range data {
        values = append(values, val - value)
    }
    return values
}


