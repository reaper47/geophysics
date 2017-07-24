import React from 'react'

const Button = ({ text, onClickEvent }) => (
  <button type="button" onClick={onClickEvent}>
    {text}
  </button>
)

export default Button

