package structs

import (
    "strings"
    "geophysics/utils/arrayutils"
)

type Worden807Struct struct {
    Stations []float64
    Comments []string
    Time []float64
    TimeMin []float64
    Readings [][]float64
    SurveyPurpose string
    SurveyArea string
    SurveyPOI string
    SurveyAddress string
    SurveyDate string
    OperationTempGrav float64
    RefStationLat float64
    GravSurveyDir float64
    AvgReading []float64
    Std []float64
    RelativeGravField []float64
    GravAnomalyNotCorrected []float64
    TemporalVariations []float64
    AttractionDerivation []float64
    LatCorrection []float64
    FreeAirCorrection []float64
    BouguerCorrection []float64
    BouguerRelativeGravField []float64
    BouguerAnomaly [] float64
    RegionalAnomaly []float64
    ResidualAnomaly []float64
}

func PushToWorden807Struct(gravStruct *Worden807Struct, data []string, field string) {
    fieldSplit := strings.Split(field, " ")
    
    if strings.Contains(fieldSplit[0], "station") {
        gravStruct.Stations = arrayutils.ConvStringToFloat64(data)
        return
    }
    
    if arrayutils.StringContainedInSlice("comment", fieldSplit) {
        gravStruct.Comments = data
        return
    }
    
    if arrayutils.StringContainedInSlice("time", fieldSplit) && !arrayutils.StringContainedInSlice("min", fieldSplit) {
       gravStruct.Time = arrayutils.ConvStringToFloat64(data)
       return
    }
    
    if arrayutils.StringContainedInSlice("time", fieldSplit) && arrayutils.StringContainedInSlice("min", fieldSplit) {
       gravStruct.TimeMin = arrayutils.ConvStringToFloat64(data)
       return
    }
    
    if arrayutils.StringContainedInSlice("reading", fieldSplit) {
        convertedData := arrayutils.ConvStringToFloat64(data)
        gravStruct.Readings = append(gravStruct.Readings, convertedData)
        return
    }
    
    if arrayutils.StringContainedInSlice("purpose", fieldSplit) {
        gravStruct.SurveyPurpose = data[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("area", fieldSplit) {
        gravStruct.SurveyArea = data[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("interest", fieldSplit) || arrayutils.StringContainedInSlice("poi", fieldSplit) {
        gravStruct.SurveyPOI = data[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("address", fieldSplit) {
        gravStruct.SurveyAddress = data[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("date", fieldSplit) {
        gravStruct.SurveyDate = data[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("operation", fieldSplit) && arrayutils.StringContainedInSlice("temp", fieldSplit) { 
        // check if temperature has to be in fahrenheit or celsius
        gravStruct.OperationTempGrav = arrayutils.ConvStringToFloat64(data)[0]
        return 
    }
    
    if arrayutils.StringContainedInSlice("lat", fieldSplit) && arrayutils.StringContainedInSlice("reference", fieldSplit) {
        gravStruct.RefStationLat = arrayutils.ConvStringToFloat64(data)[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("survey", fieldSplit) && arrayutils.StringContainedInSlice("direction", fieldSplit) {
        gravStruct.GravSurveyDir = arrayutils.ConvStringToFloat64(data)[0]
        return
    }
    
}
    
