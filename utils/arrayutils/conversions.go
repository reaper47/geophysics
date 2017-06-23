package arrayutils

import (
    "log"
    "strconv"
)

func ConvIntToFloat64(data []int) []float64 {
    var values []float64
    for _, val := range data {
        values = append(values, float64(val))
    }
    return values
}

func ConvFloat64ToInt(data []float64) []int {
    var values []int
    for _, val := range data {
        values = append(values, int(val))
    }
    return values
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
