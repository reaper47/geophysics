import React from 'react'
import ReactDOM from 'react-dom'
import { BrowserRouter } from 'react-router-dom'

const css = './assets/styles'
require(`${css}/imports.scss`)
require(`${css}/general.scss`)
require(`${css}/file_upload.scss`)
require(`${css}/header.scss`)
require(`${css}/scrollbar.scss`)

import App from './app/App.jsx'

ReactDOM.render((
  <BrowserRouter>
    <App />
  </BrowserRouter>
), document.getElementById('root'))

function requireUtils(r) { r.keys().forEach(r); }
requireUtils(require.context('./app/utils', true, /\.js$/));