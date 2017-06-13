package mathutils

func Mean2D(data [][]float64) []float64 {
    var avgs []float64
    n := arrayutils.FindMaxNbValsFloat64(data)
    m := float64(len(data))
        
    for i := 0; i < n; i++ {
        sum := 0.0
        for _, val := range data {
            sum += val[i]
        }
        avgs = append(avgs, mathutils.Round(sum / m, .5, 5))
    }
        
    return avgs
}

func Std2D(data [][]float64) []float64 {
    return 0.0
}

