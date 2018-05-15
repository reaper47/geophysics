const CLICK = 'click'
const GRID = 'grid'

const map = {
    '#home': () => renderHomePage(),
    '#gravimetry': () => renderGravimetryPage(),
    '#gravimetry/graphs?mode=import': () => renderGravimetryImportCSV(),
    '#seismology': () => renderSeismologyPage(),
    '#electricity': () => renderElectricityPage(),
    '#magnetism': () => renderMagnetismPage(),
    '#electromagnetism': () => renderElectroPage(),
    '#about': () => console.log('about')
}

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
    el.addEventListener(CLICK, (e) => {
        e.preventDefault()
        e.stopPropagation()
        window.location.href = el.href
        renderHomePage()
    })
}



let menuToggle = (el) => {
    const header = document.querySelector('header')
    const checkbox = document.getElementById('drawer-toggle')

    el.addEventListener(CLICK, (e) => {
        e.preventDefault()
        e.stopPropagation()
        header.classList.toggle('on')
        checkbox.checked = !checkbox.checked
    })
}

/*
 * Render Modules
 *
 */
let renderHomePage = () => {
    const main = document.getElementById('main-home')
    resetHeaderToWelcome()
    main.style.display = GRID
}


// Gravimetry
let renderGravimetryPage = () => {
    const main = cached['main_gravimetry']
    main.style.display = GRID
    resetCSS()
    closeModalCSS(main)
}



let renderGravimetryImportCSV = () => {
    const main = cached['main_gravimetry']
    main.style.display = GRID
    resetCSS()
    openModalCSS(main)
}

let processGravimetry = () => {
    const form = document.getElementById('grav-files-form')
    form.reset()
    window.location.hash = '#gravimetry'
    const files = document.querySelector('#grav-files-input').files
    
    console.log(files)
}


// Seismology
let renderSeismologyPage = () => {
    const main = document.getElementById('main-seismology')
    main.style.display = GRID
    underConstructionCSS()
}


// Electricity
let renderElectricityPage = () => {
    const main = document.getElementById('main-electricity')
    main.style.display = GRID
    underConstructionCSS()
}


// Magnetism
let renderMagnetismPage = () => {
    const main = document.getElementById('main-magnetism')
    main.style.display = GRID
    underConstructionCSS()
}


// Electromagnetism
let renderElectroPage = () => {
    const main = document.getElementById('main-electro')
    main.style.display = GRID
    underConstructionCSS()
}



let initHash = () => {
    let mains = document.getElementsByTagName('main')
    let mains_arr = Array.prototype.slice.call(mains)
    
    let render = (url) => {
        let to = url.split('/')
        mains_arr.map(x => x.style.display = 'none')
    
      if(to.length == 1) {
          to = to[0]
          resetCSS()
      
          const title = document.title.split(':')[0]
          let method = to.split('#')[1]
          method = method[0].toUpperCase() + method.substring(1)

          if(method === 'Home') 
              returnToWelcome(title)
          else
              navigateToMethod(title, method)
          
          map[to].apply()
      } else {
          map[url].apply()
      }
    }
  
    window.addEventListener('hashchange', () => {
        render(decodeURI(window.location.hash))
    })
}



let returnToWelcome = (title) => {
    document.title = title
    resetHeaderToWelcome()
}



let navigateToMethod = (documentTitle, method) => {
    document.getElementById('header-title').textContent = method
    document.title = `${documentTitle}: ${method}`
}



let resetCSS = () => cached['body'].style.backgroundColor = '#FFF'
let underConstructionCSS = () => cached['body'].style.backgroundColor = 'rgb(8, 8, 8)'
let resetHeaderToWelcome = () => cached['header_h1'].textContent = 'Welcome to Geophysica!'

/*
 * Modal
 *
 */
let openModalCSS = (main) => {
    cached['open_modal'].style.opacity = 1
    main.style.pointerEvents = 'none'
    main.style.userSelect = 'none'
    cached['open_modal'].style.pointerEvents = 'auto'
    cached['header'].style.pointerEvents = 'none'
}

let closeModalCSS = (main) => {
    cached['open_modal'].style.opacity = 0
    main.style.pointerEvents = 'auto'
    main.style.userSelect = 'auto'
    cached['open_modal'].style.pointerEvents = 'none'
    cached['header'].style.pointerEvents = 'auto'
}

slidingMenu();
initHash();

