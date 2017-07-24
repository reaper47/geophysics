import React from 'react'
import FilesUnorderedList from './FilesUnorderedList.jsx'

const FileUploadAside = ({ accepted, rejected }) => (
  <aside>
    <FilesUnorderedList items={accepted} title="Accepted files" />
    <FilesUnorderedList items={rejected} title="Rejected files" />
  </aside>
)

export default FileUploadAside

