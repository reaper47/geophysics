import React, { Component } from 'react'
import Dropzone from 'react-dropzone'
import Button from '../basic/Button.jsx'
import FileUploadAside from './FileUploadAside.jsx'

/*
<div id="container__file_upload">
      <h2>File Upload</h2>
      <form id="form__file_upload" action="/api/upload" method="post" encType="multipart/form-data">
        <input id="file_upload" type="file" name="fileUpload" accept=".csv" />
        <label id="file_upload_drag" htmlFor="file_upload">
          <img id="file_upload_image" src="#" alt="preview" className="hidden" />
          <div id="file_upload_start">
            <i className="fa fa-download" aria-hidden="true"></i>
            <div>Select a file or drag here</div>
            <div id="notimage" className="hidden">Please select a csv file</div>
            <span id="file_upload_submit" className="btn btn-primary">Select a file</span>
          </div>
          <div id="file_upload_response" className="hidden">
            <div id="file_upload_messages"></div>
            <progress className="progress" value="0">
              <span>0</span>%
            </progress>
          </div>
        </label>
      </form>
  </div>
*/

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
   
    return (
      <section>
        <div className="dropzone">
          <Dropzone  accept="text/csv"
                     id="form__file_upload"
                     onDrop={(accepted, rejected) => this.onDrop(accepted, rejected)}
                     ref={node => { dropzoneRef = node }}>
            {
              ({ isDragActive, isDragReject, acceptedFiles, rejectedFiles }) => {
                if(isDragActive) return "This files is authorized"
                if(isDragReject) return "This file is not authorized"
                return acceptedFiles.length || rejectedFiles.length
                  ? `Accepted ${acceptedFiles.length}, rejected ${rejectedFiles.length} files`
                  : 'Select a file or drop here.'
              }
            }
          </Dropzone>
        </div>
        
        <Button text="Open File Dialog" onClickEvent={() => dropzoneRef.open()} />
        <FileUploadAside accepted={this.state.accepted} rejected={this.state.rejected} />
      </section>
    )
  }
}

export default FileUpload

