#!/usr/bin/python
from flask import Flask, request
import pandas as pd
import requests
import time

# Se establece los valores necesarios para la conexión:
INFLUX_TOKEN='bgeXKm-T7brDq7olkrvEFt0xlK1BSwN-xhMMl0rtB8SIRfOq_OaHq3OmxNCgWIypdVP85McwcDivNSHfAcSm6w=='
ORG='UC3M'
INFLUX_URL='127.0.0.1'
BUCKET_NAME='Test_proyecto'

# Se establece la precisión en MILISEGUNDOS:
QUERY_URI='http://{}:8086/api/v2/write?org={}&bucket={}&precision=ms'.format(INFLUX_URL,ORG,BUCKET_NAME)

# Se establece la cabecera con el token correspondiente
headers = {}
headers['Authorization'] = 'Token {}'.format(INFLUX_TOKEN)
measurement_name = 'Proyecto'


app = Flask(__name__)
@app.route('/sensor_values', methods = ['GET', 'POST'])
def read_sensors():
    if request.method == 'GET':
        
        temperature = request.args.get('temperature', default = None)
        pressure = request.args.get('pressure', default = 0)
        altitude = request.args.get('altitude', default = 0)
        humidity = request.args.get('humidity', default = 0)

        data = "\tValor de Temperatura mediante GET: " + str(temperature) + " *C\n" + \
                "\tValor de Presion mediante GET: " + str(pressure) + " hPa\n" + \
                "\tValor de Altitud mediante GET: " + str(altitude) + " m\n" + \
                "\tValor de Humedad mediante GET: " + str(humidity) + " %\n", 200

        return  data

    elif request.method == 'POST':
        content = request.get_json()
        print("---------------- He recibido --------------------")
        print("Valor de Measurement mediante POST: " + str(content['measure']) + " *C")
        print("Valor de Temperatura mediante POST: " + str(content['temperature']) + " *C")
        print("Valor de Presion mediante POST: " + str(content['pressure']) + " hPa")
        print("Valor de Altitud mediante POST: " + str(content['altitude']) + " m")
        print("Valor de Humedad mediante POST: " + str(content['humidity']) + " %")
        print("-------------------------------------------------")

        

        ##measurement_data = "BME280"
        measurement_data = str(content['measure'])
        temperature_name = "Temperature"
        gas_name = "Gas"
        key_name = "RFIDKeys"
        temperature_data = str(content['temperature'])
        pressure_data = str(content['pressure'])
        altitude_data = str(content['altitude']) 
        humidity_data = str(content['humidity'])
        gas_data = str(content['gas'])
        print("KEY: " + str(content['keys']))
        key_data = str(content['keys'])

        dataE = []
        ##dataE_gas = []
        dataE.append('{name},placa={measurement} temperature={temperature},humidity={humidity},pressure={pressure},altitude={altitude}'.format(name=measurement_name,measurement=measurement_data,temperature=temperature_data, humidity=humidity_data, pressure=pressure_data, altitude=altitude_data))
        dataE.append('{name},placa={measurement} gas={gas}'.format(name=gas_name,measurement=measurement_data,gas=gas_data))
        dataE.append('{name},placa={measurement} RFIDkey={rfid_key}'.format(name=key_name,measurement=measurement_data,rfid_key=key_data))
        dataEnvio = '\n'.join(dataE)

        ##dataEnvio = '\n'.join(dataE)
        print("Envio mediante POST: " + dataEnvio)
        r = requests.post(QUERY_URI, data=dataEnvio, headers=headers)
        print(r.status_code)
        return "Recibido correctamente!\n" + \
				"\tSensor: " + str(content['measure']) + \
                "\tTemperatura: " + str(content['temperature']) + " *C\n" + \
                "\tPresion: " + str(content['pressure']) + " hPa\n" + \
                "\tAltitud: " + str(content['altitude']) + " m\n" + \
                "\tHumedad: " + str(content['humidity']) + " %\n", 200
    

app.run(host="0.0.0.0", port="5000")