package calcs

import (
    "fmt"
    "strings"
    "math"
    "geophysics/constants"
    "geophysics/structs"
    "geophysics/utils/arrayutils"
    "geophysics/utils/mathutils"
)

func InitWorden807Struct(data [][]string) *structs.Worden807Struct {
    gravStruct := new(structs.Worden807Struct)

    for i, val := range data[0] {
        colData := arrayutils.GetColData(data, i)
        structs.PushToWorden807Struct(gravStruct, colData, strings.ToLower(val))
    }
    
    return gravStruct
}

func PopulateWorden807(gravStruct *structs.Worden807Struct, topoStruct *structs.TopoStruct) {
    gravStruct.AvgReading = mathutils.Mean2D(gravStruct.Readings)
    gravStruct.Std = mathutils.PopulationStdDev2D(gravStruct.Readings)
    
    dialConstant := DialConstantCorrectionWorden807(gravStruct.OperationTempGrav, "F")
    gravStruct.RelativeGravField = arrayutils.MultiplyArrByConstant(gravStruct.AvgReading, dialConstant)
    
    referenceStations := arrayutils.GatherSameItemsIndex64(0.0, gravStruct.Stations)
    firstRelativeGravField := gravStruct.RelativeGravField[referenceStations[0]]
    gravStruct.GravAnomalyNotCorrected = arrayutils.SubtractValueFromArray(firstRelativeGravField, gravStruct.RelativeGravField)
    
    gravStruct.TemporalVariations = calculateTemporalVariations(referenceStations, gravStruct)
    gravStruct.AttractionDerivation = calculateAttractionDerivation(len(referenceStations), referenceStations, gravStruct)
    gravStruct.LatCorrection = calculateLatitudeCorrection(gravStruct)
    gravStruct.FreeAirCorrection = arrayutils.MultiplyArrByConstant(topoStruct.ElevationToRefStationCorrected, constants.FreeAir)
    gravStruct.BouguerCorrection = arrayutils.MultiplyArrByConstant(topoStruct.ElevationToRefStationCorrected, constants.RockDensity * constants.Bouguer)
    gravStruct.BouguerRelativeGravField = calculateBouguerRelativeGravField(gravStruct)
    gravStruct.BouguerAnomaly = arrayutils.SubtractValueFromArray(gravStruct.BouguerRelativeGravField[0], gravStruct.BouguerRelativeGravField)
    gravStruct.RegionalAnomaly = calculateRegionalAnomaly(gravStruct)
    
    residualAnomaly, err := arrayutils.SubtractTwoArrays(gravStruct.BouguerAnomaly, gravStruct.RegionalAnomaly)
    if err != nil {
        fmt.Println(err)
    }
    
    gravStruct.ResidualAnomaly = residualAnomaly
    
}

func DialConstantCorrectionWorden807(temp float64, unit string) float64 {
    upperY, lowerY := constants.Worden807UpperY, constants.Worden807LowerY
    
    var leftX, rightX float64
    if strings.ToLower(unit) == "f" {
        rightX, leftX = constants.Worden807RightXFahr, constants.Worden807LeftXFahr
    } else {
        rightX, leftX = constants.Worden807RightXCelsius, constants.Worden807LeftXCelsius
    }

    return ((upperY - lowerY) / (leftX - rightX)) * float64(temp) + lowerY
}

/**
 * Develop a general solution
 */
func calculateAttractionDerivation(numRefStations int, refStations []int, gravStruct *structs.Worden807Struct) []float64 {
    if numRefStations != 3 {
        fmt.Errorf("gravimetry: three reference stations must be given")
    }
    
    variations := gravStruct.TemporalVariations
    minTimes := gravStruct.TimeMin
    
    var derivations []float64
    for key, val := range minTimes {
        if key > 12 {
            diffRefStations := (minTimes[refStations[2]] - minTimes[refStations[1]])
            diffMinTimes := minTimes[key] - minTimes[refStations[1]]
            derivations = append(derivations, variations[2] * diffMinTimes / diffRefStations)
        } else {
            derivations = append(derivations, variations[1] * val / minTimes[refStations[1]])
        }
        
    }

    return derivations
}

/*
 * Develop a general solution
 */
func calculateBouguerRelativeGravField(gravStruct *structs.Worden807Struct) []float64 {
    pc1, err := arrayutils.AddTwoArrays(gravStruct.AttractionDerivation, gravStruct.LatCorrection)
    if err != nil {
        fmt.Println(err)
    }
    
    pc1 = arrayutils.RemoveElementFromSlice(pc1, 12)
    pc1 = arrayutils.RemoveElementFromSlice(pc1, 26)

    freeAirSlice := gravStruct.FreeAirCorrection[:len(gravStruct.FreeAirCorrection)/2+1]
    bouguerSlice := gravStruct.BouguerCorrection[:len(gravStruct.BouguerCorrection)/2+1]

    pc2, err := arrayutils.AddTwoArrays(freeAirSlice, bouguerSlice)
    if err != nil {
        fmt.Println(err)
    }
    
    pc3, err := arrayutils.AddTwoArrays(pc1, pc2)
    if err != nil {
        fmt.Println(err)
    }
    
    relativeGravFields := gravStruct.RelativeGravField
    relativeGravFields = arrayutils.RemoveElementFromSlice(relativeGravFields, 12)
    relativeGravFields = arrayutils.RemoveElementFromSlice(relativeGravFields, 26)

    gravField, err := arrayutils.SubtractTwoArrays(relativeGravFields, pc3)
    if err != nil {
        fmt.Println(err)
    }
    
    return gravField
}

func calculateRegionalAnomaly(gravStruct *structs.Worden807Struct) []float64 {
    stations := gravStruct.Stations
    stations = arrayutils.RemoveElementFromSlice(stations, 12)
    stations = arrayutils.RemoveElementFromSlice(stations, 26)
    stations = arrayutils.DivideArrByConstant(stations, 500)
    
    return arrayutils.MultiplyArrByConstant(stations, 0.30)    
}

func calculateTemporalVariations(refStations []int, gravStruct *structs.Worden807Struct) []float64 {
    var temporalVariations []float64
    anomalies := gravStruct.GravAnomalyNotCorrected
    
    temporalVariations = append(temporalVariations, 0)
    for i, n := 1, len(refStations); i < n; i++ {
        temporalVariations = append(temporalVariations, anomalies[refStations[i]] - anomalies[refStations[0]])        
    }
    
    return temporalVariations
}

func calculateLatitudeCorrection(gravStruct *structs.Worden807Struct) []float64 {
    latRadians := gravStruct.RefStationLat * math.Pi / 180
    longRadians := gravStruct.GravSurveyDir * math.Pi / 180
    inverseEquatorR := -0.81399
    
    var corrections []float64
    for _, val := range gravStruct.Stations {
        corrections = append(corrections, inverseEquatorR * math.Sin(2*latRadians) * math.Cos(longRadians) * val / 1000)
    }
    
    return  corrections
}

