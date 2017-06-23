package main

import (
    "fmt"
    "log"
    "net/http"
    "strconv"
    "sync"
    
    "geophysics/actions"
    "geophysics/calcs"
    "geophysics/utils/fileutils"
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
    log.Println(t2)

    grav := fileutils.GatherCSVData(fname, 15)
    t := calcs.InitWorden807Struct(grav)
    calcs.PopulateWorden807(t)
    //log.Println(t)
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

