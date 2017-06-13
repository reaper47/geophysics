package structs

import (
    "strings"
    "geophysics/utils/arrayutils"
)

type GravDataWorden807 struct {
    Station []float64
    Comments []string
    Time []float64
    TimeMin []float64
    Readings [][]float64
    SurveyPurpose string
    SurveyArea string
    SurveyPOI string
    SurveyAddress string
    SurveyDate string
    OperationTempGrav float32
    RefStationLat float32
    GravSurveyDir float32
    AvgReading []float64
    Std []float64    
    RelativeGravField []float64
    GravAnomalyNotCorrected []float64  
    TemporalVars []float64
    AttractionDeriv []float64
    LatCorrection []float64
    ElevComparedToStation []float64
    AltComparedToAvgSea []float64
    FreeAirCorr []float64
    BouguerCorr []float64
    GravFieldRelBouguer []float64
    BouguerAnomaly [] float64
    RegionalAnomaly []float64
    ResidualAnomaly []float64
}

func PushToWorden807Struct(gravStruct *GravDataWorden807, data []string, field string) {
    fieldSplit := strings.Split(field, " ")
    
    if strings.Contains(fieldSplit[0], "station") {
        gravStruct.Station = arrayutils.ConvStringToFloat64(data)
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
       gravStruct.Time = arrayutils.ConvStringToFloat64(data)
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
        gravStruct.OperationTempGrav = arrayutils.ConvStringToFloat32(data)[0]
        return 
    }
    
    if arrayutils.StringContainedInSlice("lat", fieldSplit) && arrayutils.StringContainedInSlice("reference", fieldSplit) {
        gravStruct.RefStationLat = arrayutils.ConvStringToFloat32(data)[0]
        return
    }
    
    if arrayutils.StringContainedInSlice("survey", fieldSplit) && arrayutils.StringContainedInSlice("direction", fieldSplit) {
        gravStruct.GravSurveyDir = arrayutils.ConvStringToFloat32(data)[0]
        return
    }
    
}
    
