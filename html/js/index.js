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
        checkbox.checked = !checkbox.checked
    })
}

let renderHomePage = () => {
    const main = document.getElementById('main-home')
    resetHeaderToWelcome()
    main.style.display = 'grid'
}

let renderGravimetryPage = () => {
    const main = document.getElementById('main-gravimetry')
    main.style.display = 'grid'
}

let renderSeismologyPage = () => {
    const main = document.getElementById('main-gravimetry')
    main.style.display = 'grid'
}

let renderElectricityPage = () => {
    const main = document.getElementById('main-electricity')
    main.style.display = 'grid'
}

let renderMagnetismPage = () => {
    const main = document.getElementById('main-magnetism')
    main.style.display = 'grid'
}

let renderElectroPage = () => {
    const main = document.getElementById('main-electro')
    main.style.display = 'grid'
}

let initHash = () => {
    let mains = document.getElementsByTagName('main')
    let mains_arr = Array.prototype.slice.call(mains)
  
    let render = (url) => {
        const to = url.split('/')[0]
        mains_arr.map(x => x.style.display = 'none')
    
        const map = {
            '#home': () => renderHomePage(),
            '#gravimetry': () => renderGravimetryPage(),
            '#seismology': () => renderSeismologyPage(),
            '#electricity': () => renderElectricityPage(),
            '#magnetism': () => renderMagnetismPage(),
            '#electro': () => renderElectroPage(),
            '#about': () => console.log('about')
      }
            
      if(map[to]) {
          const title = document.title.split(':')[0]
          let method = to.split('#')[1]
          method = method[0].toUpperCase() + method.substring(1)
          document.title = `${title}: ${method}`
          
          if(method === 'Home') {
              resetHeaderToWelcome()
          } else {
              const header_h1 = document.getElementById('header-title')
              header_h1.textContent = method
          }
          
          map[to].apply()
      } else {
          console.log('error')
      }
    }
  
    window.addEventListener('hashchange', () => {
        render(decodeURI(window.location.hash))
    })
}

let resetHeaderToWelcome = () => {
    const header_h1 = document.getElementById('header-title')
    header_h1.textContent = 'Welcome to Geophysica!'
}

slidingMenu();
initHash();
