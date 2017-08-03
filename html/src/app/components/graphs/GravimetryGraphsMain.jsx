import React, { Component } from 'react'
import * as d3 from 'd3'
import {withFauxDOM} from 'react-faux-dom'

class GravimetryGraphsMain extends Component {
  
  constructor() {
    super()
    
    this.data = JSON.parse(localStorage.getItem('gravimetricData')).gravimetricData
    this.renderD3 = this.renderD3.bind(this)
    this.xyChartD3 = this.xyChartD3.bind(this)
  }
  
  componentDidMount () {
    this.renderD3()
  }

  xyChartD3(faux) {
      let width = 960
      let height = 480
      let xlabel = 'Residual Anomaly (mGal)'
      let ylabel = 'Distance (m)'
      
      function chart(selection) {
        selection.each(datasets => {
          const margin = {top: 20, right: 20, bottom: 30, left: 50}
          const innerWidth = width - margin.left - margin.right
          const innerHeight = height - margin.top - margin.bottom
          
          const xScale = d3.scaleLinear()
            .range([0, innerWidth])
            .domain([d3.min(datasets, d => d3.min(d.x)), 
                     d3.max(datasets, d => d3.max(d.x))])
          
          const yScale = d3.scaleLinear()
            .range([innerHeight, 0])
            .domain([d3.min(datasets, d => d3.min(d.y)),
                     d3.max(datasets, d => d3.max(d.y))])
          
          const colorScale = d3.scaleOrdinal(d3.schemeCategory10)
            .domain(d3.range(datasets.length))
            
          const xAxis = d3.axisBottom(xScale)
          const yAxis = d3.axisLeft(yScale)
          
          const xGrid = d3.axisBottom(xScale)
            .tickSize(-innerHeight)
            .tickFormat('')
          
          const yGrid = d3.axisLeft(yScale)
            .tickSize(-innerWidth)
            .tickFormat('')
            
          const drawLine = d3.line()
            .x(d => xScale(d[0]))
            .y(d => yScale(d[1]))
            .curve(d3.curveCatmullRom.alpha(0.5))
              
          const graphSVG = d3.select(faux).append('svg')
            .attr("width", 1200)
            .attr("height", 600)
            .attr('class', 'gravimetryGraph')
            .attr("preserveAspectRatio", 'none')
            .append("g")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
            
          graphSVG.append('g')
            .attr('class', 'x grid')
            .attr('transform', 'translate(0,' + innerHeight + ')')
            .call(xGrid)
            
          graphSVG.append('g')
            .attr('class', 'y grid')
            .call(yGrid)
          
          graphSVG.append('g')
            .attr('class', 'x axis')
            .attr('transform', 'translate(0,' + innerHeight + ')')
            .call(xAxis)
            .append('text')
            .attr('dy', '-.71rem')
            .attr('x', 448)
            .attr('y', 64)
            .style('text-anchor', 'end')
            .style('fill', '#212121')
            .text(xlabel)
            
          graphSVG.append('g')
            .attr('class', 'y axis')
            .call(yAxis)
            .append('text')
            .attr('transform', 'rotate(-90)')
            .attr('x', -171)
            .attr('y', -45)
            .style('text-anchor', 'end')
            .style('fill', '#212121')
            .text(ylabel)
          
          const dataLines = graphSVG.selectAll('.xyChartLineD3')
            .data(datasets.map(d => d3.zip(d.x, d.y)))
            .enter().append('g')
            .attr('class', 'xyChartLineD3')
            
          dataLines.append('path')
            .attr('class', 'line')
            .attr('d', d => drawLine(d))
            .attr('stroke', (_, i) => colorScale(i))
            
          dataLines.append('text')
            .datum(function(d, i) { return {name: datasets[i].label, final: d[d.length-1]}; })
            .attr('transform', d => (`translate(${xScale(d.final[0])}, ${yScale(d.final[1])})`))
            .attr('x', 3)
            .attr('dy', '.35rem')
            .attr('fill', (_, i) => colorScale(i))
            .text(d => d.name)
        })
      }
      
      chart.width = value => {
        if (!arguments.length) return width
        width = value
        return chart
      }
      
      chart.height = value => {
        if (!arguments.length) return height
        height = value
        return chart
      }
      
      chart.xlabel = value => {
        if (!arguments.length) return xlabel
        xlabel = value
        return chart
      }
      
      chart.ylabel = value => {
        if (!arguments.length) return ylabel
        ylabel = value
        return chart
      }
      
      return chart
    }  

  renderD3() {
    const faux = this.props.connectFauxDOM('div', 'chart')
    
    const xData = this.data.stations
    const yData = this.data.bouguer_anomaly
    for (let i = 0, n = xData.length; i <= n; i++) {
      if (i === 0 || i === 11 || i === 25) {
        xData.splice(i, 1)
        yData.splice(i, 1)
      }
    }
     
    const data = [
      {
        label: "Observed residual anomaly",
        x: xData,
        y: yData
      }
    ]
  
    const xyChart = this.xyChartD3(faux)
      .width(960)
      .height(500)
      .xlabel("Residual Anomaly (mGal)")
      .ylabel("Distance (m)")
      
    const graph = d3.select(faux)
      .datum(data)
      .call(xyChart)
      
    this.props.animateFauxDOM(800)
  }

  render() {
    return (
      <div>
        <h2>Bouguer Anomaly</h2>
        <div className='renderedD3'>
          {this.props.chart}
        </div>
      </div>
    )
  }
}

GravimetryGraphsMain.defaultProps = {
  chart: 'loading'
}

export default withFauxDOM(GravimetryGraphsMain)

