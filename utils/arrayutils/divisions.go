package arrayutils

func DivideArrByConstant(data []float64, constant float64) []float64 {
	var arr []float64
	for i := range data {
		arr = append(arr, data[i]/constant)
	}
	return arr
}
