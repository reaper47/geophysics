package arrayutils

import (
	"errors"
	"geophysics/constants"
)

func SubtractArrayOneIndexAhead(data []float64, add []float64, appendZero bool) ([]float64, error) {
	if len(data) != len(add) {
		return nil, errors.New(constants.ArraysNotSameLength)
	}

	var values []float64
	if appendZero {
		values = append(values, 0.0)
	}

	for i, n := 1, len(data); i < n; i++ {
		values = append(values, data[i-1]-data[i]+add[i])
	}

	return values, nil
}

func SubtractSameArrayOneIndexAhead(data []float64) []float64 {
	var values []float64
	values = append(values, 0.0)

	for i, n := 1, len(data); i < n; i++ {
		values = append(values, values[i-1]-data[i])
	}

	return values
}

func SubtractTwoArrays(a []float64, b []float64) ([]float64, error) {
	if len(a) != len(b) {
		return nil, errors.New(constants.ArraysNotSameLength)
	}

	var c []float64
	for i, n := 0, len(a); i < n; i++ {
		c = append(c, a[i]-b[i])
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
		values = append(values, b[i]-a[i-1])
	}
	return values, nil
}

func SubtractValueFromArray(value float64, data []float64) []float64 {
	var values []float64
	for i := range data {
		values = append(values, data[i] - value)
	}
	return values
}

