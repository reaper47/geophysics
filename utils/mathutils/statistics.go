package mathutils

import (
	"geophysics/utils/arrayutils"
	"math"
)

func Mean2D(data [][]float64) []float64 {
	var avgs []float64
	n := arrayutils.FindMaxNbValsFloat64(data)
	m := float64(len(data))

	for i := 0; i < n; i++ {
		sum := 0.0
		for j := range data {
			sum += data[j][i]
		}
		avgs = append(avgs, Round(sum/m, .5, 5))
	}

	return avgs
}

func PopulationStdDev2D(data [][]float64) []float64 {
	var stdDevs []float64
	avgs := Mean2D(data)
	n := arrayutils.FindMaxNbValsFloat64(data)
	m := len(data)

	for i := 0; i < n; i++ {
		deviation := 0.0
		currentAvg := avgs[i]

		for j := range data {
			deviation += math.Pow(math.Abs(data[j][i]-currentAvg), 2)
		}

		stdDevs = append(stdDevs, math.Sqrt(deviation/float64(m)))
	}

	return stdDevs
}
