package structs

import (
	"strconv"
	"strings"
	"geophysics/utils/arrayutils"
)

type Worden807Struct struct {
	Stations                 []float64
	Comments                 []string
	Time                     []float64
	TimeMin                  []float64
	Readings                 [][]float64
	AvgReading               []float64
	Std                      []float64
	RelativeGravField        []float64
	GravAnomalyNotCorrected  []float64
	TemporalVariations       []float64
	AttractionDerivation     []float64
	LatCorrection            []float64
	FreeAirCorrection        []float64
	BouguerCorrection        []float64
	BouguerRelativeGravField []float64
	BouguerAnomaly           []float64
	RegionalAnomaly          []float64
	ResidualAnomaly          []float64
	SurveyPurpose            string
	SurveyArea               string
	SurveyPOI                string
	SurveyAddress            string
	SurveyDate               string
	OperationTempGrav        float64
	RefStationLat            float64
	GravSurveyDir            float64
}

func CreateCSVHeadersWorden807(gravStruct *Worden807Struct) []string {
	headers := []string{"Station", "Comments", "Time (hh:mm)", "Time (mm)"}
	chunk := []string{"Average Reading", "Standard Deviation", "Relative Gravitational Field (mGal)",
		"Uncorrected Gravitational Anomaly in Relation to Reference Station (mGal)",
		"Temporal Variations (mGal)", "Instrumental and Attraction Derivation (mGal)",
		"Latitude Correction (m)", "Elevation in Relation to Reference Station (m)",
		"Altitude in Relation to Average Sea Level (m)", "Free Air Correction (mGal)",
		"Bouguer Correction (mGal)", "Bouguer Relative Gravitational Field (mGal)",
		"Bouguer Anomaly in Relation to Reference Station (mGal)",
		"Regional Anomaly (mGal)", "Residual Anomaly (mGal)"}

	for i, n := 0, len(gravStruct.Readings); i < n; i++ {
		headers = append(headers, "Reading "+strconv.Itoa(i+1))
	}

	for i, n := 0, len(chunk); i < n; i++ {
		headers = append(headers, chunk[i])
	}

	return headers
}

func FetchAllDataWorden807(gravStruct *Worden807Struct) [][]string {
    var fetched [][]string 
        
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.Stations))
    fetched = append(fetched, gravStruct.Comments)
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.Time))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.TimeMin))
    
    for _, subarr := range gravStruct.Readings {
        fetched = append(fetched, arrayutils.ConvFloat64ToStr(subarr))
    }
    
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.AvgReading))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.Std))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.RelativeGravField))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.GravAnomalyNotCorrected))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.TemporalVariations))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.AttractionDerivation))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.LatCorrection))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.FreeAirCorrection))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.BouguerCorrection))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.BouguerRelativeGravField))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.BouguerAnomaly))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.RegionalAnomaly))
    fetched = append(fetched, arrayutils.ConvFloat64ToStr(gravStruct.ResidualAnomaly))
    
    var singles []string
    singles = append(singles, gravStruct.SurveyPurpose)
    singles = append(singles, gravStruct.SurveyArea)
    singles = append(singles, gravStruct.SurveyPOI)
    singles = append(singles, gravStruct.SurveyAddress)
    singles = append(singles, gravStruct.SurveyDate)
    singles = append(singles, strconv.FormatFloat(gravStruct.OperationTempGrav, 'f', 18, 64))
    singles = append(singles, strconv.FormatFloat(gravStruct.RefStationLat, 'f', 18, 64))
    singles = append(singles, strconv.FormatFloat(gravStruct.GravSurveyDir, 'f', 18, 64))
    
    for _, val := range singles {
        singleEntreeArr := make([]string, len(gravStruct.Stations))
        for i, n := 0, len(gravStruct.Stations); i < n; i++ {
            if i == 0 {
                singleEntreeArr[i] = val
            } else {
                singleEntreeArr[i] = ""
            }
        }
        fetched = append(fetched, singleEntreeArr)
    }
    
	return fetched
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

