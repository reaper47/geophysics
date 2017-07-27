import React, { Component } from 'react'
import Dropzone from 'react-dropzone'
import Button from '../basic/Button.jsx'
import FileUploadAside from './FileUploadAside.jsx'
import 'jsx-control-statements'


class FileUpload extends Component {
  constructor() {
    super()
    
    this.state = { 
      accepted: [],
      rejected: []
    }
  }
  
  onDrop(accepted, rejected) {
    this.setState({ accepted, rejected }, () => this.uploadToServer())
  }
  
  uploadToServer() {
    for(let i = 0, n = this.state.accepted.length; i < n; i++) {
      const xhr = new XMLHttpRequest()
      const file = this.state.accepted[i]
      let data = new FormData()
      
      data.append('file', file, file.name)
      xhr.open('POST', '/api/openCSV', true);
      xhr.send(data);
    }
  }
    
  render() {  
    let dropzoneRef
   
    const dropzoneStyle = {
      background: "#F9F9F9",
      border: "2px dashed #000",
      height: "48rem",
      width: "48rem",
      paddingTop: "5rem"
    }
     
    const activeStyle = {
      borderColor: "#2980b9",
    } 
    
    const rejectStyle = {
      borderColor: "#c0392b"
    }
   
    return (
      <div id="form__upload_div">
        <section id="form__upload_section">
          <div className="dropzone">
            <div id="form__upload_help">

            </div>
            <Dropzone  accept="text/csv"
                       activeStyle ={activeStyle}
                       id="form__file_upload"
                       onDrop={(accepted, rejected) => this.onDrop(accepted, rejected)}
                       ref={node => { dropzoneRef = node }}
                       rejectStyle={rejectStyle}
                       style={dropzoneStyle}>
               {
               ({isDragActive, isDragReject, acceptedFiles, rejectedFiles}) => {
                  if(isDragActive) return (
                      <div>
                        <Choose>
                          <When condition={rejectedFiles.length > 1}>
                            <p>These files are authorized.</p>
                          </When>
                          <Otherwise>
                            <p>This file is authorized.</p>
                          </Otherwise>
                        </Choose>         
                        <svg style={{fill: '#2980b9'}} width="25rem" height="25rem" viewBox="0 0 24 24"><path d="M19.35 10.04C18.67 6.59 15.64 4 12 4 9.11 4 6.6 5.64 5.35 8.04 2.34 8.36 0 10.91 0 14c0 3.31 2.69 6 6 6h13c2.76 0 5-2.24 5-5 0-2.64-2.05-4.78-4.65-4.96zM10 17l-3.5-3.5 1.41-1.41L10 14.17 15.18 9l1.41 1.41L10 17z"/></svg>
                      </div>
                  )
                  
                  if(isDragReject) return (
                      <div>
                        <Choose>
                          <When condition={rejectedFiles.length > 1}>
                            <p>These files are not authorized.</p>
                          </When>
                          <Otherwise>
                            <p>This file is not authorized.</p>
                          </Otherwise>
                        </Choose>                
                        <svg style={{fill: '#c0392b'}} width="25rem" height="25rem" viewBox="0 0 24 24"><path d="M19.35 10.04C18.67 6.59 15.64 4 12 4c-1.48 0-2.85.43-4.01 1.17l1.46 1.46C10.21 6.23 11.08 6 12 6c3.04 0 5.5 2.46 5.5 5.5v.5H19c1.66 0 3 1.34 3 3 0 1.13-.64 2.11-1.56 2.62l1.45 1.45C23.16 18.16 24 16.68 24 15c0-2.64-2.05-4.78-4.65-4.96zM3 5.27l2.75 2.74C2.56 8.15 0 10.77 0 14c0 3.31 2.69 6 6 6h11.73l2 2L21 20.73 4.27 4 3 5.27zM7.73 10l8 8H6c-2.21 0-4-1.79-4-4s1.79-4 4-4h1.73z"/></svg>
                      </div>
                  )
                  
                  return acceptedFiles.length || rejectedFiles.length
                    ? (
                        <div>
                          <p>Accepted {acceptedFiles.length} {acceptedFiles.length > 1 ? 'files' : 'file'}, rejected {rejectedFiles.length} {rejectedFiles.length > 1 ? 'files' : 'file'}</p>
                          <svg style={{fill: '#27ae60'}} width="25rem" height="25rem" viewBox="0 0 24 24"><path d="M19.35 10.04C18.67 6.59 15.64 4 12 4 9.11 4 6.6 5.64 5.35 8.04 2.34 8.36 0 10.91 0 14c0 3.31 2.69 6 6 6h13c2.76 0 5-2.24 5-5 0-2.64-2.05-4.78-4.65-4.96zM14 13v4h-4v-4H7l5-5 5 5h-3z"/></svg>
                        </div>                        
                      )
                    : (
                        <div>
                          <p>Click the button or drop files here</p>
                          <svg width="25rem" height="25rem" viewBox="0 0 24 24"><path d="M19.35 10.04C18.67 6.59 15.64 4 12 4 9.11 4 6.6 5.64 5.35 8.04 2.34 8.36 0 10.91 0 14c0 3.31 2.69 6 6 6h13c2.76 0 5-2.24 5-5 0-2.64-2.05-4.78-4.65-4.96zM14 13v4h-4v-4H7l5-5 5 5h-3z"/></svg>
                        </div>
                      )
                }
              }
            </Dropzone>
          </div>
          <Button text="Open File Dialog" onClickEvent={() => dropzoneRef.open()} />
        </section>
        <FileUploadAside accepted={this.state.accepted} rejected={this.state.rejected} />
      </div>
    )
  }
}

export default FileUpload

