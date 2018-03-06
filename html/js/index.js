let slidingMenu = () => {
  const headerToggle = document.querySelector("header");
  const labelToggle = document.getElementById("drawer-toggle-label");

  menuToggle(headerToggle);
  menuToggle(labelToggle);
}

let menuToggle = (el) => {
  const header = document.querySelector("header");
  const checkbox = document.getElementById("drawer-toggle");

  el.addEventListener("click", (e) => {
    e.preventDefault();
    e.stopPropagation();
    header.classList.toggle("on");
    checkbox.checked = !checkbox.checked
  })
}

slidingMenu();
