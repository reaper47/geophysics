package main

import (
	"fmt"
	"encoding/json"
	"geophysics/actions"
	"geophysics/calcs"
	"geophysics/utils/fileutils"
	"log"
	"net/http"
	"strconv"
	"sync"
)

var counter int
var mutex = &sync.Mutex{}

func incrementCounter(w http.ResponseWriter, r *http.Request) {
	mutex.Lock()
	counter++
	fmt.Fprintf(w, strconv.Itoa(counter))
	mutex.Unlock()
}

func serveSingle(pattern string, filename string) {
	http.HandleFunc(pattern, func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, filename)
	})
}

func test(w http.ResponseWriter, r *http.Request) {
	fname := "data/uploads/grav.csv"
	fname2 := "data/uploads/topo.csv"

	topo := fileutils.GatherCSVData(fname2, 5)
	t2 := calcs.InitTopoStruct(topo)
	calcs.PopulateTopoStruct(t2, 11, 99.0)

	grav := fileutils.GatherCSVData(fname, 15)
	t := calcs.InitWorden807Struct(grav)
	calcs.PopulateWorden807(t, t2)
	fileutils.WriteGravStructToCSV(',', false, "ehllo.csv", t)
	
	body, err := json.Marshal(t)
	if err != nil {
	    panic(err)
	}
	
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write(body)
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("html/public")))
	http.HandleFunc("/api/openCSV", test)

	serveSingle("/sitemap.xml", "./sitemap.xml")
	serveSingle("/favicon.ico", "./favicon.ico")
	serveSingle("/robots.txt", "./robots.txt")

	http.HandleFunc("/increment", incrementCounter)
	http.HandleFunc("/api/upload", actions.Upload)

	log.Fatal(http.ListenAndServe(":8080", nil))
}
