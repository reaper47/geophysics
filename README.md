# Geophysics: Bouguer Anomaly
*Geophysics: Bouguer Anomaly* is a web application used to help geophysicists 
calculate the Bouguer anomaly from their gravimetric surveys. 

## Supported gravimeters
Currently supported gravimeters are:
* Worden 807

## High-level functionality
The user is expected to upload a csv file of their survey data having field 
names adhering to the standard defined in the upload section of the application. 
Column order is not important, but field names are. Once uploaded, the file is 
sent to the server, is processed through an endless chain of functions, and is 
sent back to the user. It can then be downloaded for further studies. 

Furthermore, the application generates and displays three graphs from the data. 
The first being an interactive graph of the Bouguer anomaly where parameter 
knobs can be adjusted as to align the actual data curve on the model. 
Once aligned the user can export the graph with the parameter knob values for 
further studies. The second is a static graph displaying distance of the survey 
vs gravimetric values. The third and final one, for the moment, displays 
distance of the survey vs the altitude. A csv file containing topography data 
has to be uploaded. Including this information under the gravimetric survey 
data csv file is under consideration.

## Low-level functionality
Details to come...

## Development Methodology
Development of this software follows test-driven development practices with
[MinUnit][0], a minimal unit testing framework for C/C++. 

A test is first written and failed every time a new function has to be created.
Then, the function associated to the test is written and its test turns green. 
This cycle continues throughout the development process. 
Commits are only pushed if all tests pass.

Test files are stored under [/v2/tests/][1].

[0]: https://github.com/siu/minunit
[1]: https://github.com/reaper47/geophysics/tree/master/v2/tests
