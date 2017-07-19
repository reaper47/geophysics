package arrayutils

import (
	"errors"
	"geophysics/constants"
)

func AddArrWithConstant(data []float64, constant float64) []float64 {
	var arr []float64
	for i := range data {
		arr = append(arr, data[i]+constant)
	}
	return arr
}

func AddTwoArrays(a []float64, b []float64) ([]float64, error) {
	if len(a) != len(b) {
		return nil, errors.New(constants.ArraysNotSameLength)
	}

	var c []float64
	for i, n := 0, len(a); i < n; i++ {
		c = append(c, a[i]+b[i])
	}

	return c, nil
}
