package calcs

import (
    "strings"
    "geophysics/structs"
    "fmt"
    "geophysics/utils/arrayutils"
)

func InitTopoStruct(data [][]string) *structs.TopoStruct {
    topoStruct := new(structs.TopoStruct)

    for i, val := range data[0] {
        colData := arrayutils.GetColData(data, i)
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
    
    lastElevToRef := -topoStruct.ElevationDifferenceCorrected[len(topoStruct.ElevationToRefStation)-1] / 2
    errorStations := arrayutils.DivideArrByConstant(arrayutils.ConvIntToFloat64(topoStruct.StationOrder), 50)
    topoStruct.ErrorDistributionBtwnStations = arrayutils.MultiplyArrByConstant(errorStations, lastElevToRef)
    
    topoStruct.ElevationDifferenceCorrected = elevationDiffCorrected
    
    elevToRefStationCorr, err := arrayutils.AddTwoArrays(topoStruct.ElevationToRefStation, topoStruct.ErrorDistributionBtwnStations)
    if err != nil {
        fmt.Println(err)
    }
    topoStruct.ElevationToRefStationCorrected = elevToRefStationCorr
    //topoStruct.AltitudeToAvgSeaLevel =
}

