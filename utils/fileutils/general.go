package fileutils

import (
    "os"
    "log"
)

func OpenFile(filepath string) *os.File {
    file, err := os.Open(filepath)
    if err != nil {
      log.Printf("Failed to open the file: %s", filepath)
      return nil
    }
    return file
}
