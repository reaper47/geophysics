const cached = {
    'body': document.querySelector('body'),
    'header_h1': document.getElementById('header-title'),
    'header': document.querySelector('header'),
    'open_modal': document.getElementById('open_modal'),
    'main_gravimetry': document.getElementById('main-gravimetry')
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
    return newData
}

class Graph {
    constructor(opts) {
        this.originalData = opts.originalData
        this.data = cleanData(opts.data)
        this.element = opts.element
        this.numGridTicks = 8
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
        
        this.createScale()
        this.addAxes()
        this.addLine()
        this.deleteTopAxisLine()
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
        
        this.plot.append('g')
            .attr('class', 'y axis grid_graph')
            .call(this.yAxis)
    }
    
    addLine() {
        const line = d3.line()
            .x(d => this.xScale(d[0]))
            .y(d => this.yScale(d[1]))

        this.plot.append('path')
            .datum(this.data)
            .classed('line', true)
            .attr('d', line)
            .style('stroke', this.lineColor || 'red')
    }
    
    deleteTopAxisLine() {
        let paths = document.getElementsByTagName('path')
        paths[5].remove()
    }
    
    /* Public Methods */
    setColor(newColor) {
        this.plot.select('.line').style('stroke', newColor)
    }
    
    setData(newData) {
        this.data = cleanData(newData)
        this.draw()
    }
    
    getData() {
        return this.originalData
    }
}

slidingMenu();

