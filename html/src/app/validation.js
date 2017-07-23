function validateFileType(file, targets) {
  return targets.includes(file.type)
}

function validateFileSize(file, maxSize) {
  if(file.size === 0 || file.size > maxSize) {
    return false
  }
  return true
}

function validateSizeAndType(file) {
  const oneGb = 1073741824
  const supportedTypes = ['text/csv']
  
  sizeValidated = validateFileSize(file, oneGb)
  typeValidated = validateFileType(file, supportedTypes)

  if(!sizeValidated || !typeValidated) {
    alert('Only non-empty csv files are supported.')
    return false
  }
  
  return true
}

