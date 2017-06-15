package fileutils

import (
    "os"
    "log"
    "bufio"
    "encoding/csv"
    "geophysics/utils/arrayutils"
)

func GatherCSVData(fname string, addFields int) [][]string {
    delimiter := detectDelimiter(fname)

    file := OpenFile(fname)
    r := csv.NewReader(file)
    r.Comma = delimiter
    
    records, err := r.ReadAll()
    if err != nil {
        log.Fatal(err)
    }
 
    if addFields > 0 {
        for key, value := range records {
            records[key] = arrayutils.ResizeSlice(value, len(value) + addFields)
        }
    }
    
    file.Close()
    return records
}

func detectDelimiter(fname string) rune {
    f := OpenFile(fname)
         
    m := createCSVMap()
    s := bufio.NewScanner(f)
    
    for s.Scan() {
        countCSVRunes(s.Bytes(), m)
    }
    
    if err := s.Err(); err != nil {
        log.Println(os.Stderr, "scanning f failed")
    }

    f.Close()
    
    var maxRune rune
    maxInt := 0
    for k := range m {
        if m[k] > maxInt {
            maxInt = m[k]
            maxRune = k
        }
    }
    
    return maxRune
}

func countCSVRunes(data []byte, m map[rune]int) {
    quotes := false
    for _, val := range data {
        if val == 34 { // quotes
            if !quotes {
                quotes = true
            } else if quotes {
                quotes = false
            }
        } else if !quotes && val == 9 {
            m[' ']++
        } else if !quotes && val == 44 {
            m[',']++
        } else if !quotes && val == 59 {
            m[';']++
        } else if !quotes && val == 124 {
            m['|']++
        }
    }
}

func createCSVMap() map[rune]int {
    m := make(map[rune]int)
    m[','] = 0
    m['|'] = 0
    m[';'] = 0
    m[' '] = 0
    return m
}

