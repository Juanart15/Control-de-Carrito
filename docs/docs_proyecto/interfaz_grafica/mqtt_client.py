import os
import json
import paho.mqtt.client as mqtt
from dotenv import load_dotenv

load_dotenv()

MQTT_HOST = os.getenv("MQTT_HOST")
MQTT_PORT = int(os.getenv("MQTT_PORT"))
MQTT_USER = os.getenv("MQTT_USER")
MQTT_PASS = os.getenv("MQTT_PASS")
TOPIC_SENSORS = os.getenv("TOPIC_SENSORS")

last_sensor_data = None

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT:", rc)
    client.subscribe(TOPIC_SENSORS)

def on_message(client, userdata, msg):
    global last_sensor_data
    try:
        last_sensor_data = json.loads(msg.payload.decode())
        print("Sensor data:", last_sensor_data)
    except:
        print("Invalid sensor payload")

def start_mqtt():
    client = mqtt.Client()
    client.username_pw_set(MQTT_USER, MQTT_PASS)
    client.tls_set()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(MQTT_HOST, MQTT_PORT)
    client.loop_start()
    return client
