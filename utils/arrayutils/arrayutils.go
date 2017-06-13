package arrayutils

import (
    "log"
    "strconv"
    "strings"
)

func ConvStringToFloat32(data []string) []float32 {
    var convData []float32
    for _, val := range data {
        if val == "" {
            convData = append(convData, 0.0)
            continue
        }
        
        conv, err := strconv.ParseFloat(val, 32)
        if err != nil {
	        log.Println("conversion error in convToFloat32: ", err)
        }       
        convData = append(convData, float32(conv))
    }
    return convData
}

func ConvStringToFloat64(data []string) []float64 {
    var convData []float64
    for _, val := range data {
        if val == "" {
            convData = append(convData, 0.0)
            continue
        }
        
        conv, err := strconv.ParseFloat(val, 64)
        if err != nil {
	        log.Println("conversion error in convToFloat64: ", err)
        }
        convData = append(convData, conv)
    }
    return convData
}

func ConvStringToInt(data []string) []int {
    var convData []int
    for _, val := range data {
        conv, err := strconv.Atoi(val)
        if err != nil {
	        log.Println("conversion error in convToInt: ", err)
        }
        convData = append(convData, conv)
    }
    return convData
}

func GetColData(data [][]string, i int) []string {
    var colData []string
    for _, val := range data {
        colData = append(colData, val[i])
    }
    return colData[1:]
}

func ResizeSlice(data []string, expand int) []string {
    if (len(data) + expand) > cap(data) {
        newSlice := make([]string, len(data), cap(data) + expand)
        copy(newSlice, data)
        return newSlice
    }
    return data
}

func StringContainedInSlice(a string, arr []string) bool {
    for _, b := range arr {
        if strings.Contains(b, a) {
            return true
        }
    }
    return false
}

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

