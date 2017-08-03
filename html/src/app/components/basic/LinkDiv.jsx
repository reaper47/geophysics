import React from 'react'
import { Link } from 'react-router-dom'
import 'jsx-control-statements'

const LinkDiv = ({ id, linkTo, title, instruction, proceedText, onClickFnc }) => (
  <div id={id}>
    <h2>{title}</h2>
    <div className={`${id}_text`}>
      <p>{instruction}</p>
    </div>
    <div className={`${id}_bottom`}>
      <div className={`${id}_bottom_left`}></div>
      <Link to={linkTo} onClick={onClickFnc}>{proceedText}</Link>
    </div>
  </div>
)

export default LinkDiv
