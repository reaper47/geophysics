function fileDragHover(e) {
  let fileDrag = document.getElementById('file_upload_drag')
  
  e.stopPropagation()
  e.preventDefault()

  fileDrag.className = (e.type === 'dragover' ? 'hover' : 'modal_body file_upload')
}

function fileSelectHandler(e) {
  fileDragHover(e);
  
  const files = e.target.files || e.dataTransfer.files
  for (let i = 0, f; f = files[i]; i++) {
    if(parseFile(f)) uploadFile(f)
  }
}

function parseFile(file) {
  if(!validateSizeAndType(file)) {
    return false
  }
  
  const fileName = file.name
  output('<strong>' + encodeURI(fileName) + '</strong>')
  let isGood = (/\.(?=csv)/gi).test(fileName)
  
  if (isGood) {
    document.getElementById('file_upload_start').classList.add("hidden")
    document.getElementById('file_upload_response').classList.remove("hidden")
    document.getElementById('notimage').classList.add("hidden")
    
    // Thumbnail Preview
    document.getElementById('file_image').classList.remove("hidden")
    document.getElementById('file_image').src = 'https://unsplash.it/200/?random'
  }
  else {
    document.getElementById('file_upload_image').classList.add("hidden")
    document.getElementById('notimage').classList.remove("hidden")
    document.getElementById('file_upload_start').classList.remove("hidden")
    document.getElementById('file_upload_response').classList.add("hidden")
    document.getElementById("form__file_upload").reset()
  }
  return true
}

function setProgressMaxValue(e) {
  let pBar = document.getElementById('file_progress')

  if (e.lengthComputable) {
    pBar.max = e.total;
  }
}

function updateFileProgress(e) {
  let pBar = document.getElementById('file_progress')

  if (e.lengthComputable) {
    pBar.value = e.loaded
  }
}

function uploadFile(file) {
  let xhr = new XMLHttpRequest()
  let pBar = document.getElementById('file_progress')
  let fileSizeLimit = 1024; // In MB
  
  let form = document.getElementById('form__file_upload')
  let formData = new FormData(form)
  formData.append('file', file, file.name)
  
  if (xhr.upload) {
    if (file.size <= fileSizeLimit * 1024 * 1024) {
      pBar.style.display = 'inline'
      xhr.upload.addEventListener('loadstart', setProgressMaxValue, false)
      xhr.upload.addEventListener('progress', updateFileProgress, false)

      xhr.open('POST', document.getElementById('form__file_upload').action, true);
      xhr.send(formData);
    } else {
      output('Please upload a smaller file (< ' + fileSizeLimit + ' MB).');
    }
  }
}

function output(msg) {
  const m = document.getElementById('messages')
  m.innerHTML = msg
}

function uploadInit() {
  const fileSelect = document.getElementById('file_upload')
  const fileDrag = document.getElementById('file_upload_drag')
  const submitButton = document.getElementById('file_upload_submit')

  fileSelect.addEventListener('change', fileSelectHandler, false)

  let xhr = new XMLHttpRequest()
  if (xhr.upload) {
    fileDrag.addEventListener('dragover', fileDragHover, false)
    fileDrag.addEventListener('dragleave', fileDragHover, false)
    fileDrag.addEventListener('drop', fileSelectHandler, false)
  }
}

