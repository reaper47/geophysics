package fileutils

import (
	"bufio"
	"encoding/csv"
	"geophysics/constants"
	"geophysics/structs"
	"geophysics/utils/arrayutils"
	"log"
	"os"
)

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
		} else if !quotes && val == 32 {
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
	m[' '] = 0
	return m
}

func GatherCSVData(fname string, addFields int) [][]string {
	delimiter := detectDelimiter(fname)

	if delimiter == ' ' {
		log.Fatal("No spaces please.")
	}

	file := OpenFile(fname)
	r := csv.NewReader(file)
	r.Comma = delimiter

	records, err := r.ReadAll()
	if err != nil {
		log.Fatal(err)
	}

	if addFields > 0 {
		for key, value := range records {
			records[key] = arrayutils.ResizeSlice(value, len(value)+addFields)
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

func WriteGravStructToCSV(delimiter rune, useCRLF bool, filename string, gravStruct *structs.Worden807Struct) {
	out, err := os.Create(constants.WriteToCSVFilePathGrav + filename)
	if err != nil {
		log.Fatal(constants.CreatingFile, err)
	}

	w := csv.NewWriter(out)
	w.Comma = delimiter
	w.UseCRLF = useCRLF

	defer func() {
		w.Flush()
		out.Close()
	}()

	var buffer [][]string

	headers := structs.CreateCSVHeadersWorden807(gravStruct)
	buffer = append(buffer, headers)

	wordenData := structs.FetchAllDataWorden807(gravStruct)
	//wordenData = arrayutils.InvertArrayStr(wordenData)

	for i := range wordenData {
		buffer = append(buffer, wordenData[i])
	}

	for i := range buffer {
		w.Write(buffer[i])
	}

}
