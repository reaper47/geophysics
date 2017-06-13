package fileutils

import (
    "os"
    "log"
    "encoding/csv"
    "geophysics/utils/arrayutils"
)

func GatherCSVData(file *os.File, addFields int) [][]string {
    r := csv.NewReader(file)
    
    records, err := r.ReadAll()
    if err != nil {
        log.Fatal(err)
    }
 
    if addFields > 0 {
        for key, value := range records {
            records[key] = arrayutils.ResizeSlice(value, len(value) + addFields)
        }
    }
 
    return records
}

