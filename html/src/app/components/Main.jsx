import React from 'react'
import { Switch, Route } from 'react-router-dom'
import Home from './Home.jsx'
import Gravimetry from './Gravimetry.jsx'

const Main = () => (
  <main>
    <Switch>
      <Route exact path='/' component={Home} />
      <Route path='/gravimetry' component={Gravimetry} />
    </Switch>
  </main>
)

export default Main

