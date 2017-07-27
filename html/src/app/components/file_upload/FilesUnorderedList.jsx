import React from 'react'
import 'jsx-control-statements'

const FilesUnorderedList = ({ items, title }) => (
  <div>
    <h2>{title}</h2>
    <ul>
      <For each="item" index="i" of={items}>
        <li key={item.name} className="file_upload_item">{item.name} - {item.size} bytes</li>
      </For>
    </ul>    
  </div>
)

export default FilesUnorderedList

