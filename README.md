# Geophysics: Bouguer Anomaly
*Geophysics: Bouguer Anomaly* is a web application used to help geophysicians calculate the Bouguer anomaly from their gravimetric surveys. 

## Supported gravimeters
Currently supported gravimeters are:
* Worden 807

## High-level functionality
The user is expected to upload a csv file of their survey data having field names adhering to the standard defined in the upload section of the application. Column order is not important, but field names are. Once uploaded, the file is sent to the server, is processed through an endless chain of functions, and is sent back to the user. It can then be downloaded for further studies. 

Furthermore, the application generates and displays three graphs from the data. The first being an interactive graph of the Bouguer anomaly where parameter knobs can be adjusted as to align the actual data curve on the model. Once aligned the user can export the graph with the parameter knob values for further studies. The second is a static graph displaying distance of the survey vs gravimetric values. The third and final one, for the moment, displays distance of the survey vs the altitude. A csv file containing topography data has to be uploaded. Including this information under the gravimetric survey data csv file is under consideration.

## Low-level functionality
Details to come...

## TODO
[ ] Rewrite the way data is parsed when a user uploads a file

