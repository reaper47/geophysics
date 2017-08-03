import React from 'react'
import ReactDOM from 'react-dom'
import { BrowserRouter } from 'react-router-dom'
import App from './app/App.jsx'

const css = './assets/styles'
require(`${css}/imports.scss`)
require(`${css}/general.scss`)
require(`${css}/file_upload.scss`)
require(`${css}/file_upload_button.scss`)
require(`${css}/file_upload_toGraphs_btn.scss`)
require(`${css}/header.scss`)
require(`${css}/scrollbar.scss`)
require(`${css}/icons.scss`)
require(`${css}/graphs.scss`)


ReactDOM.render((
  <BrowserRouter>
    <App />
  </BrowserRouter>
), document.getElementById('mount'))

