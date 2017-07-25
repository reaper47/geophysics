package structs

import (
	"geophysics/utils/arrayutils"
	"strconv"
	"strings"
)

type Worden807Struct struct {
	Stations                 []float64   `json:"stations"`
	Comments                 []string    `json:"comments"`
	Time                     []float64   `json:"time"`
	TimeMin                  []float64   `json:"time_min"`
	Readings                 [][]float64 `json:"readings"`
	AvgReading               []float64   `json:"avg_reading"`
	Std                      []float64   `json:"std"`
	RelativeGravField        []float64   `json:"relative_grav_field"`
	GravAnomalyNotCorrected  []float64   `json:"grav_anomaly_not_corrected"`
	TemporalVariations       []float64   `json:"temporal_variations"`
	AttractionDerivation     []float64   `json:"attraction_derivation"`
	LatCorrection            []float64   `json:"lat_correction"`
	FreeAirCorrection        []float64   `json:"free_air_correction"`
	BouguerCorrection        []float64   `json:"bouguer_correction"`
	BouguerRelativeGravField []float64   `json:"bouguer_relative_grav_field"`
	BouguerAnomaly           []float64   `json:"bouguer_anomaly"`
	RegionalAnomaly          []float64   `json:"regional_anomaly"`
	ResidualAnomaly          []float64   `json:"residual_anomaly"`
	SurveyPurpose            string      `json:"survey_purpose"`
	SurveyArea               string      `json:"survey_area"`
	SurveyPOI                string      `json:"survey_POI"`
	SurveyAddress            string      `json:"survey_address"`
	SurveyDate               string      `json:"survey_date"`
	OperationTempGrav        float64     `json:"operation_temp_grav"`
	RefStationLat            float64     `json:"ref_station_lat"`
	GravSurveyDir            float64     `json:"grav_survey_dir"`
}

func CreateCSVHeadersWorden807(gravStruct *Worden807Struct) []string {
	headers := []string{"Station", "Comments", "Time (hh:mm)", "Time (mm)"}
	chunk := []string{"Average Reading", "Standard Deviation", "Relative Gravitational Field (mGal)",
		"Uncorrected Gravitational Anomaly in Relation to Reference Station (mGal)",
		"Temporal Variations (mGal)", "Instrumental and Attraction Derivation (mGal)",
		"Latitude Correction (m)", "Elevation in Relation to Reference Station (m)",
		"Altitude in Relation to Average Sea Level (m)", "Free Air Correction (mGal)",
		"Bouguer Correction (mGal)", "Bouguer Relative Gravitational Field (mGal)",
		"Bouguer Anomaly in Relation to Reference Station (mGal)", "Regional Anomaly (mGal)",
		"Residual Anomaly (mGal)", "Purpose", "Area", "POI", "Address", "Date of Survey",
		"Operation Temperature (F)"}

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
