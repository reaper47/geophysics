package calcs

import (
    "fmt"
    "strings"
    "math"
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

func PopulateWorden807(gravStruct *structs.Worden807Struct) {
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
    //gravStruct.ElevComparedToStation = 
    
    
    //gravStruct.AltComparedToAvgSea
    //gravStruct.FreeAirCorrection
    //gravStruct.BouguerCorrection 
    //gravStruct.GravFieldRelBouguer 
    //gravStruct.BouguerAnomaly 
    //gravStruct.RegionalAnomaly 
    //gravStruct.ResidualAnomaly 
    
}

func DialConstantCorrectionWorden807(temp float64, unit string) float64 {
    upperY, lowerY := 0.40546, 0.40514
    
    var leftX, rightX float64
    if strings.ToLower(unit) == "f" {
        rightX, leftX = 0.0, 120.0
    } else {
        rightX, leftX = -17.7778, 48.8889
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

