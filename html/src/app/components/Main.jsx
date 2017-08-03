import React from 'react'
import { Switch, Route } from 'react-router-dom'
import Home from './Home.jsx'
import Gravimetry from './Gravimetry.jsx'
import GravimetryGraphsMain from './graphs/GravimetryGraphsMain.jsx'

const Main = () => (
  <main>
    <Switch>
      <Route exact path='/' component={Home} />
      <Route exact path='/gravimetry/graphs' component={GravimetryGraphsMain} />
      <Route path='/gravimetry' component={Gravimetry} />
    </Switch>
  </main>
)

export default Main

