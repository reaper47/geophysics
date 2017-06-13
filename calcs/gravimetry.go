package calcs

import (
    "strings"
    "geophysics/structs"
    "geophysics/utils/arrayutils"
    "geophysics/utils/mathutils"
)

func InitGravStructWorden807(data [][]string) *structs.GravDataWorden807 {
    gravStruct := new(structs.GravDataWorden807)

    for i, val := range data[0] {
        colData := arrayutils.GetColData(data, i)
        structs.PushToWorden807Struct(gravStruct, colData, strings.ToLower(val))
    }
    
    return gravStruct
}

func CalcBouguerAnomaly(gravStruct *structs.GravDataWorden807) {
    gravStruct.AvgReading = mathutils.Mean2D(gravStruct.Readings)
    gravStruct.Std = mathutils.Std2D(gravStruct.Readings)
}

