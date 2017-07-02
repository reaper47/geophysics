package arrayutils

import (
    "log"
	"strconv"
)

func ConvIntToFloat64(data []int) []float64 {
	var arr []float64
	for i := range data {
		arr = append(arr, float64(data[i]))
	}
	return arr
}

func ConvFloat64ToInt(data []float64) []int {
	var arr []int
	for i := range data {
		arr = append(arr, int(data[i]))
	}
	return arr
}

func ConvFloat64ToStr(data []float64) []string {
    var arr []string
    for i := range data {
        arr = append(arr, strconv.FormatFloat(data[i], 'f', 18, 64))
    }
    return arr
}

func ConvStringToFloat64(data []string) []float64 {
	var values []float64
	for i := range data {
		if data[i] == "" {
			values = append(values, 0.0)
			continue
		}

		val, err := strconv.ParseFloat(data[i], 64)
		if err != nil {
			log.Println("conversion error in convToFloat64: ", err)
		}
		values = append(values, val)
	}
	return values
}

func ConvStringToInt(data []string) []int {
	var values []int
	for i := range data {
		conv, err := strconv.Atoi(data[i])
		if err != nil {
			log.Println("conversion error in convToInt: ", err)
		}
		values = append(values, conv)
	}
	return values
}

