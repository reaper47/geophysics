package calcs

import (
	"fmt"
	"geophysics/structs"
	"geophysics/utils/arrayutils"
	"strings"
)

func InitTopoStruct(data [][]string) *structs.TopoStruct {
	topoStruct := new(structs.TopoStruct)

	for i, val := range data[0] {
		colData := arrayutils.GetColumnData(data, i)
		structs.PushToTopoStruct(topoStruct, colData, strings.ToLower(val))
	}

	return topoStruct
}

func PopulateTopoStruct(topoStruct *structs.TopoStruct, refStation int, altitudeRefStation float64) {
	elevationDiffCorrected, err := arrayutils.SubtractTwoArraysOneIndexAhead(topoStruct.Zero, topoStruct.ElevationDifference)
	if err != nil {
		fmt.Println(err)
	}
	topoStruct.ElevationDifferenceCorrected = elevationDiffCorrected

	topoStruct.ElevationToRefStation = arrayutils.SubtractSameArrayOneIndexAhead(topoStruct.ElevationDifferenceCorrected)
	topoStruct.ErrorDistributionBtwnStations = calculateErrorDistributionBtwnStations(topoStruct)

	elevToRefStationCorr, err := arrayutils.AddTwoArrays(topoStruct.ElevationToRefStation, topoStruct.ErrorDistributionBtwnStations)
	if err != nil {
		fmt.Println(err)
	}
	topoStruct.ElevationToRefStationCorrected = elevToRefStationCorr

	altitudeConstant := altitudeRefStation - topoStruct.ElevationToRefStationCorrected[refStation]
	topoStruct.AltitudeToAvgSeaLevel = arrayutils.AddArrWithConstant(topoStruct.ElevationToRefStationCorrected, altitudeConstant)
}

func calculateErrorDistributionBtwnStations(topoStruct *structs.TopoStruct) []float64 {
	secondToLastElevation := topoStruct.ElevationToRefStation[len(topoStruct.ElevationToRefStation)/2-1]
	returnTripElevations := topoStruct.ElevationDifference[len(topoStruct.ElevationDifference)/2:]
	lastElevToRef := topoStruct.ElevationDifferenceCorrected[len(topoStruct.ElevationDifferenceCorrected)/2]

	var elevationDifferences []float64
	elevationDifferences = append(elevationDifferences, lastElevToRef)
	for i := range returnTripElevations {
		elevationDifferences = append(elevationDifferences, returnTripElevations[i])
	}

	elevationStationRefs := make([]float64, len(elevationDifferences))
	for i, n := 0, len(elevationDifferences)-1; i < n; i++ {
		if i > 0 {
			elevationStationRefs[i+1] = elevationStationRefs[i] - elevationDifferences[i+1]
		} else {
			elevationStationRefs[i] = 0.0
			elevationStationRefs[i+1] = secondToLastElevation - lastElevToRef
		}
	}
	elevationStationRefs = arrayutils.RemoveElementFromSlice(elevationStationRefs, len(elevationStationRefs)-1)

	errorStations := arrayutils.DivideArrByConstant(arrayutils.ConvIntToFloat64(topoStruct.StationOrder), -500)
	return arrayutils.MultiplyArrByConstant(errorStations, lastElevToRef)
}
