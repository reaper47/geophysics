package actions

import (
    "io"
    "os"
    "net/http"
)

func Upload(w http.ResponseWriter, r *http.Request) { 
    var uploadDirectory = "./data/uploads/"
               
    switch r.Method {
        case "GET":
            //display(w, "upload", nil)
            
        case "POST":            
            reader, err := r.MultipartReader()
            if err != nil {
                http.Error(w, err.Error(), http.StatusInternalServerError)
                return
            }
                   
            for {
                part, err := reader.NextPart()
                if err == io.EOF {
                    break
                }

                if part.FileName() == "" {
                    continue
                }
                
                if part.Header.Get("Content-Type") != "text/csv" && part.FileName() != "" {
                    //display(w, "save", "unsupported file type")
                    break
                }
                
                dst, err := os.Create(uploadDirectory + part.FileName())
                defer dst.Close()
                
                if err != nil {
                    http.Error(w, err.Error(), http.StatusInternalServerError)
                    return
                }
                
                if _, err := io.Copy(dst, part); err != nil {
                    http.Error(w, err.Error(), http.StatusInternalServerError)
                    return
                }
            } 
            //display(w, "save", "Upload successful")
            
        default:
            w.WriteHeader(http.StatusMethodNotAllowed)
    }
}

