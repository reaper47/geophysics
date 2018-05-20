import os
import subprocess
import json
from flask import render_template, request, session, Response
from flask import send_from_directory, url_for, redirect, jsonify
from app import app
from werkzeug.utils import secure_filename

ALLOWED_EXTENSIONS = set(['csv'])

@app.route('/<path:path>')
def static_proxy(path):
    return app.send_static_file(path)

@app.route('/')
@app.route('/index')
def index():
    modules = [
        ('home-item1', 'gravimetry', 'Astronaut suspended in microgravity above Earth.'),
        ('home-item2', 'seismology', 'Damaged building from an earthquake.'), 
        ('home-item3', 'electricity', 'A Powerful thunderstorm.'), 
        ('home-item4', 'magnetism', 'Magnetic attraction.'), 
        ('home-item5', 'electromagnetism', 'Electromagnetism.')]
    return render_template('index.html', modules=modules)

@app.route('/gravimetry')
def gravimetry():
    modules = [
        ('Import CSV', 'CSV file selection.', '#modal-gravimetry-import'),
        ('Web Mode', 'Access web editing.', 'gravimetry/web'),
        ('Load saved data set', 'Load previous data set.', '#modal-gravimetry-load')]
    return render_template('gravimetry.html', title='Gravimetry', modules=modules)

@app.route('/gravimetry/upload', methods=['GET', 'POST'])
def gravimetry_upload():
    if request.method == 'POST':
        if 'file' not in request.files:
            print("No files :(")
            return redirect(url_for('gravimetry'))

        files = request.files.getlist('file')
        valid_files = []
        for f in files:
            if f and allowed_file(f.filename):
                #filename = secure_filename(f.filename)
                #f.save(os.path.join('./', filename))
                valid_files.append(f)

        if len(valid_files) == 2:
            json_data, csv_path = process_gravimetric_files(valid_files)
            print(list(json.loads(json_data).keys()))
            return render_template('jumpto.html', json_data=json_data, jumpto='graphs')
        else:
            print("Too many files. Please upload a single topographic file and a single gravimetric file.")

    return redirect(url_for('gravimetry'))

def allowed_file(filename):
    return '.' in filename and \
        filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

def process_gravimetric_files(files):
    program = ['./bin/geophysics', './data/uploads/grav.csv', './data/uploads/topo.csv']
    result = subprocess.run(program, stdout=subprocess.PIPE).stdout.decode('utf-8')
    split = result.split('|')
    json_result = json.dumps(json.loads(split[0]))
    csv_path = split[1]
    return json_result, csv_path

@app.route('/gravimetry/graphs')
def gravimetry_graphs():
    xopts = [
        ('stations', 'Stations'), 
        ('times_min', 'Time')]

    yopts = [
        ('readings_avg', 'Average Readings'), 
        ('std', 'Standard Deviation'),
        ('rel_grav_fields', 'Relative Gravitational Field'), 
        ('grav_anom_uncorr', 'Uncorrected Gravimetric Anomaly'),
        ('attract_dev', 'Attraction/Instrumental Deviation'),
        ('lat_corr', 'Corrected Latitude'),
        ('elev', 'Elevation'), 
        ('alts', 'Sea-level Altitude'),
        ('free_air_corr', 'Free Air Correction'),
        ('bouguer_corr', 'Bouguer Correction'),
        ('bouguer_rel', 'Relative Bouguer Gravitational Field'),
        ('bouguer_anom', 'Bouguer Anomaly'),
        ('regional_anom', 'Regional Anomaly'),
        ('residual_anom', 'Residual Anomaly')]

    vol_slider_params = { 'min': -3000, 'max': -400, 'step': 1, 'default': -1400 }
    thickness_slider_params = { 'min': 1, 'max': 151, 'step': 1, 'default': 76}
    width_slider_params = { 'min': 1, 'max': 411, 'step': 1, 'default': 206}
    depth_slider_params = { 'min': 1, 'max': 200, 'step': 1, 'default': 100 }
    starting_slider_params = { 'min': -364, 'max': 624, 'step': 1, 'default': 130 }

    residual_anom_control = [
        ('Volumetric mass contrast (kg/m^3)', 'vol', vol_slider_params), 
        ('Prism thickness (m)', 'e', thickness_slider_params), 
        ('Prism horizontal width (m)', 'l', width_slider_params),
        ('Prism burial depth (m)', 'z', depth_slider_params),
        ('Starting prism position to reference station (m)', 'x', starting_slider_params)]

    return render_template('gravimetry_graphs.html', 
            title='Gravimetry: Graphs', 
            xopts=xopts, yopts=yopts, 
            residual_anom_control=residual_anom_control)

@app.route('/seismology')
def seismology():
    return render_template('seismology.html', title='Seismology', black=True)

@app.route('/electricity')
def electricity():
    return render_template('electricity.html', title='Electricity', black=True)

@app.route('/magnetism')
def magnestism():
    return render_template('magnetism.html', title='Magnetism', black=True)

@app.route('/electromagnetism')
def electromagnetism():
    return render_template('electromagnetism.html', title='Electromagnetism', black=True)

@app.route('/about')
def about():
    return render_template('electromagnetism.html', title='About', black=True)

@app.route('/robots.txt')
def static_from_root():
    return send_from_directory(app.static_folder, request.path[1:])


