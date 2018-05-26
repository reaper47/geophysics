const cached = {
    'body': document.querySelector('body'),
    'header_h1': document.getElementById('header-title'),
    'header': document.querySelector('header'),
    'open_modal': document.getElementById('open_modal'),
    'main_gravimetry': document.getElementById('main-gravimetry'),
    'G': 6.67*(10**-11),
    'residual_line': 'residual-line'
}

let slidingMenu = () => {
    const headerToggle = document.querySelector('header')
    const labelToggle = document.getElementById('drawer-toggle-label')
    const home_icon = document.getElementById('icon_home')
    
    goHome(home_icon)
    menuToggle(headerToggle)
    menuToggle(labelToggle)
}

let goHome = (el) => {
    el.addEventListener('click', (e) => {
        e.preventDefault()
        e.stopPropagation()
        window.location.href = el.href
        renderHomePage()
    })
}

let menuToggle = (el) => {
    const header = document.querySelector('header')
    const checkbox = document.getElementById('drawer-toggle')

    el.addEventListener('click', (e) => {
        e.preventDefault()
        e.stopPropagation()
        header.classList.toggle('on')
        menu_slide_main()
        checkbox.checked = !checkbox.checked
    })
}

let menu_slide_main = () => {
    if (cached['main_gravimetry'] !== null)
        cached['main_gravimetry'].classList.toggle('grav-menu-slide')
}

let cleanData = (data) => {
    let newData = data.x.reduce((acc, curr, idx) => [...acc, [curr, data.y[idx]]], [])
	newData.splice(12, 1)
	newData.splice(26, 1)
	
	interpolatedData = []
	for(let i = 0; i < newData.length - 1; ++i)
	    interpolatedData.push(interpolate(newData[i], newData[i+1]))
	    
	firstpt = newData[0]
	interpolatedData.unshift([[firstpt[0], firstpt[1]]])
	lastpt = newData[newData.length - 1]
	interpolatedData.push([[lastpt[0], lastpt[1]]])
	
    return flatten(interpolatedData)
}

let flatten = (arr) => {
    flattened = []
    arr.forEach(el => el.forEach(sub => flattened.push(sub)))
    return flattened
}

let interpolate = (pt1, pt2) => {
    const slope = (pt2[1] - pt1[1]) / (pt2[0] - pt1[0])
    let interpolateValue = pt1[1]
    
    interpolatedPoints = []
    for (let i = pt1[0] + 1; i < pt2[0]; ++i) {
        interpolatedPoints.push([i, interpolateValue + slope])
        interpolateValue += slope
    }
    
    return interpolatedPoints
}

let setColorPickerValue = (id, color) => {
    const colorPicker = document.getElementById(id)
    colorPicker.value = color
}

let updateLineColor = (event) => chart.setColor(event.target.value)

let updateChartXAxisChange = () => {
    chart.setData({ 
        x: chart.getData()[xSelectBox.value], 
        y: chart.getData()[ySelectBox.value] 
    }, getXYLabels(xSelectBox, ySelectBox))
}

let updateChartYAxisChange = () => {
    const controls = document.getElementById('graph-control-gravimetry')

    chart.setData({ 
        x: chart.getData()[xSelectBox.value], 
        y: chart.getData()[ySelectBox.value] 
    }, getXYLabels(xSelectBox, ySelectBox))
    
    if(ySelectBox.value === 'residual_anom') {
        controls.style.display = 'grid'
        document.getElementsByClassName(cached['residual_line'])[0].style.display = 'block'
    } else {
        controls.style.display = 'none'
        document.getElementsByClassName(cached['residual_line'])[0].style.display = 'none'
    }
}

let getXYLabels = (xbox, ybox) => {
    const labels = {
        x: xbox.options[xbox.selectedIndex].text,
        y: ybox.options[ybox.selectedIndex].text
    }
    
    addUnitToLabels(labels)
    return labels
}

let addUnitToLabels = (labels) => {
    const xlabel = labels.x.toLowerCase()
    if(xlabel === 'stations')
        labels.x += ' (m)'
    else if(xlabel === 'time')
        labels.x += ' (min)'
        
    const ylabel = labels.y.toLowerCase()
    if(ylabel === 'elevation' || ylabel === 'sea-level altitude')
        labels.y += ' (m)'
    else if(ylabel === 'average readings')
        labels.y += ' (div)'
    else
        labels.y += ' (mGal)'
}

class Graph {
    constructor(opts) {
        this.originalData = opts.originalData
        this.data = cleanData(opts.data)
        this.element = opts.element
        this.numGridTicks = 8
        this.xlabel = opts.xlabel
        this.ylabel = opts.ylabel
        this.resAnomParams = { G: 6.67 * (10**-11), p: -1000, e: 5, l: 100, z: 5, x: 5.0 }
        this.draw()
        
        const self = this
        window.addEventListener('resize', self.draw())
    }
    
    draw() {
        this.width = this.element.offsetWidth
        this.height = this.width/2
        this.margin = {
            top: 20,
            right: 75,
            bottom: 45,
            left: 50
        }
                
        this.element.innerHTML = ''
        const svg = d3.select(this.element).append('svg')
            .attr('preserveAspectRatio', `none`)
            .attr('class', 'svg-content')
            
        this.plot = svg.append('g')
            .attr('transform',`translate(${this.margin.left},${this.margin.top})`)
        
        this.addSecondLineToData()
        this.createScale()
        this.addAxes()
        this.addLine()
        this.addResidualPlotLine()
        this.deleteTopAxisLine()
    }
    
    addSecondLineToData() {
        const G = this.resAnomParams.G
        const p = this.resAnomParams.p
        const e = this.resAnomParams.e
        const l = this.resAnomParams.l
        const z = this.resAnomParams.z
        const x = this.resAnomParams.x

        let ypoints = []
        for (let i = -364; i < 1000; i++) 
            ypoints.push(2*G*e*p * (Math.atan((l - i) / z) + Math.atan(i / z))*100000)

        ypoints = ypoints.slice(x, this.data.length);
        ypoints.forEach((el, idx) => this.data[idx].push(el))
    }
    
    createScale() {
        const m = this.margin
        
        const xExtent = d3.extent(this.data, d => d[0])
        const yExtent = d3.extent(this.data, d => d[1])
        
        this.xScale = d3.scaleTime()
            .range([0, this.width - m.right])
            .domain(xExtent).nice()
            
        this.yScale = d3.scaleLinear()
            .range([this.height - m.top + m.bottom, 0])
            .domain(yExtent).nice()
    }
    
    addAxes() {
        const m = this.margin
    
        this.xAxis = d3.axisBottom()
            .scale(this.xScale)
            .ticks(this.numGridTicks)
            .tickSize(-this.height - m.top, 0, 0)
            .tickFormat(d => d * 1.0)
            
        this.yAxis = d3.axisLeft()
            .scale(this.yScale)
            .ticks(this.numGridTicks)
            .tickSize(-this.width + m.right)
   
        this.plot.append('g')
            .attr('class', 'x axis grid_graph')
            .attr('transform',`translate(0, ${this.height - m.top + m.bottom})`)
            .call(this.xAxis)
                    
        this.plot.append('text')
		    .attr('x', this.width / 2 )
            .attr('y',  this.height + this.margin.bottom + 15)
            .style('text-anchor', 'middle')
            .style('font-size', '0.9em')
            .classed('x-label', true)
            .text(this.xlabel)
        
        this.plot.append('g')
            .attr('class', 'y axis grid_graph')
            .call(this.yAxis)
            
		this.plot.append('text')
            .attr('transform', 'rotate(-90)')
            .attr('y', -this.margin.left)
            .attr('x', -this.height / 2)
            .attr('dy', "1em")
            .style("text-anchor", "middle")
            .style('font-size', '0.9em')
            .classed('y-label', true)
            .text(this.ylabel)
    }
    
    addLine() {
        const line = d3.line()
            .x(d => this.xScale(d[0]))
            .y(d => this.yScale(d[1]))

        this.plot.append('path')
            .classed('line', true)
            .attr("class", "line")
		    .attr("d", line(this.data));
    }
    
    addResidualPlotLine() {
        const m = this.margin
        
        const line = d3.line()
            .x(d => this.xScale(d[0]))
            .y(d => this.yScale(d[2]))
        
        this.plot.append('path')
            .classed('residual_line', true)
            .attr("class", cached['residual_line'])
		    .style("stroke", "red")
		    .attr("d", line(this.data));
    }
    
    deleteTopAxisLine() {
        let paths = document.getElementsByTagName('path')
        paths[5].remove()
    }
    
    /* Public Methods */
    setColor(newColor) {
        this.lineColor = newColor
        this.plot.select('.line').style('stroke', newColor)
    }
        
    setData(newData, labels) {
        this.data = cleanData(newData)
        this.xlabel = labels.x
        this.ylabel = labels.y
        this.draw()
    }
    
    setResidualAnomalyData(newParams) {
        this.resAnomParams.p = newParams.p
        this.resAnomParams.e = newParams.e
        this.resAnomParams.l = newParams.l
        this.resAnomParams.z = newParams.z
        this.resAnomParams.x = newParams.x
        
        let newArr = []
        for(let i = 0; i < this.data.length; ++i)
            newArr.push(this.data[i].splice(0, 2))
        this.data = newArr
        
        this.addSecondLineToData()
        this.draw()
    }
    
    getData() {
        return this.originalData
    }
}

slidingMenu();

