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
		for _, val := range data {
			sum += val[i]
		}
		avgs = append(avgs, Round(sum/m, .5, 5))
	}

	return avgs
}

func PopulationStdDev2D(data [][]float64) []float64 {
	var stdDevs []float64
	avgs := Mean2D(data)
	n := arrayutils.FindMaxNbValsFloat64(data)
	numEls := len(data)

	for i := 0; i < n; i++ {
		deviation := 0.0
		currentAvg := avgs[i]

		for _, val := range data {
			deviation += math.Pow(math.Abs(val[i]-currentAvg), 2)
		}

		stdDevs = append(stdDevs, math.Sqrt(deviation/float64(numEls)))
	}

	return stdDevs
}
