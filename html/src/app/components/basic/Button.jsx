import React from 'react'

const Button = ({ text, onClickEvent }) => (
  <button type="button" onClick={onClickEvent}>
    <span>{text}</span>
  </button>
)

export default Button

