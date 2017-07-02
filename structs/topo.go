package structs

import (
	"geophysics/utils/arrayutils"
	"strings"
)

type TopoStruct struct {
	Stations                       []float64
	StationOrder                   []int
	ElevationDifference            []float64
	Zero                           []float64
	SurveyPurpose                  string
	SurveyArea                     string
	POI                            string
	Address                        string
	DateOfSurvery                  string
	Comments                       []string
	ElevationDifferenceCorrected   []float64
	ElevationToRefStation          []float64
	ErrorDistributionBtwnStations  []float64
	ElevationToRefStationCorrected []float64
	AltitudeToAvgSeaLevel          []float64
}

func PushToTopoStruct(topoStruct *TopoStruct, data []string, field string) {
	fieldSplit := strings.Split(field, " ")

	if strings.Contains(fieldSplit[0], "station") {
		topoStruct.Stations = arrayutils.ConvStringToFloat64(data)
		return
	}

	if arrayutils.StringContainedInSlice("order", fieldSplit) {
		topoStruct.StationOrder = arrayutils.ConvStringToInt(data)
		return
	}

	if arrayutils.StringContainedInSlice("elevation", fieldSplit) {
		topoStruct.ElevationDifference = arrayutils.ConvStringToFloat64(data)
		return
	}

	if arrayutils.StringContainedInSlice("zero", fieldSplit) {
		topoStruct.Zero = arrayutils.ConvStringToFloat64(data)
		return
	}

	if arrayutils.StringContainedInSlice("purpose", fieldSplit) {
		topoStruct.SurveyPurpose = data[0]
		return
	}

	if arrayutils.StringContainedInSlice("area", fieldSplit) {
		topoStruct.SurveyArea = data[0]
		return
	}

	if arrayutils.StringContainedInSlice("poi", fieldSplit) || arrayutils.StringContainedInSlice("interest", fieldSplit) {
		topoStruct.POI = data[0]
		return
	}

	if arrayutils.StringContainedInSlice("address", fieldSplit) {
		topoStruct.Address = data[0]
		return
	}

	if arrayutils.StringContainedInSlice("date", fieldSplit) {
		topoStruct.DateOfSurvery = data[0]
		return
	}

	if arrayutils.StringContainedInSlice("comment", fieldSplit) {
		topoStruct.Comments = data
		return
	}

}
