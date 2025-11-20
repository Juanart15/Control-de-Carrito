from flask import Flask, jsonify, request
from flask_cors import CORS
import paho.mqtt.client as mqtt
import os

app = Flask(__name__)
CORS(app)

# MQTT Configuration
MQTT_BROKER = "f2aecae8cf734a3ab0b875bb11e4b3b7.s1.eu.hivemq.cloud"
MQTT_PORT = 8883
MQTT_USER = "usuarioesp32"
MQTT_PASS = "Password1"
MOVEMENT_TOPIC = "carro/control"

# TLS + MQTT Connection
mqtt_client = mqtt.Client()
mqtt_client.username_pw_set(MQTT_USER, MQTT_PASS)
mqtt_client.tls_set()
mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
mqtt_client.loop_start()

# ------------------ API ENDPOINTS ------------------

@app.route('/api/v1/healthcheck', methods=['GET'])
def healthcheck():
    return jsonify({"status": "OK", "mqtt": "connected"})

@app.route('/api/v1/move', methods=['POST'])
def move_robot():
    data = request.json

    # Debe venir un comando EXACTO como usa tu ESP32
    command = data.get("command")

    if command is None:
        return jsonify({"error": "Missing 'command' field"}), 400

    # Publicar al topic EXACTO del ESP32
    mqtt_client.publish(MOVEMENT_TOPIC, command)
    
    return jsonify({"sent": command})

if __name__ == '__main__':
    app.run(debug=True)
