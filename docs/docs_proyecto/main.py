from fastapi import FastAPI
import paho.mqtt.client as mqtt
import requests
import os

# ========================
# CONFIGURACIÓN GLOBAL
# ========================

MQTT_SERVER = "f2aecae8cf734a3ab0b875bb11e4b3b7.s1.eu.hivemq.cloud"
MQTT_PORT = 8883
MQTT_USER = "usuarioesp32"
MQTT_PASS = "Password1"

MQTT_TOPIC_MOV = "carro/movimiento"
MQTT_TOPIC_CTRL = "carro/control"

ESP32_IP = "http://192.168.2.3"   # <-- Cambiar si tu ESP32 cambia IP

# ========================
# MQTT CLIENT (TLS)
# ========================

client = mqtt.Client()
client.username_pw_set(MQTT_USER, MQTT_PASS)
client.tls_set()  # Usa certificados por defecto (válido para HiveMQ Cloud)
client.connect(MQTT_SERVER, MQTT_PORT)

# ========================
# FASTAPI
# ========================

app = FastAPI()


# ======================================
# 1) HEALTHCHECK
# ======================================
@app.get("/api/v1/healthcheck")
def healthcheck():
    try:
        res = requests.get(ESP32_IP + "/api/v1/healthcheck", timeout=2)
        esp32_status = res.json()
    except:
        esp32_status = {"status": "offline"}

    return {
        "api_status": "ok",
        "mqtt_connected": True,
        "esp32": esp32_status
    }


# ======================================
# 2) CONTROLAR EL ROBOT (POST)
# ======================================
@app.post("/api/v1/move")
def move_robot(command: str):
    """
    Recibe comandos como:
    "forward 200 2"
    "backward 150 1"
    "left 255 1"
    "right 255 1"
    """

    # Enviar al ESP32 por HTTP (opcional)
    try:
        requests.post(ESP32_IP + "/api/v1/move", data=command)
    except:
        pass

    # Publicar por MQTT (principal)
    client.publish(MQTT_TOPIC_CTRL, command)

    return {"status": "command_sent", "command": command}


# ======================================
# 3) OBTENER ÚLTIMO COMANDO (GET)
# ======================================
@app.get("/api/v1/move")
def get_last_move():
    try:
        res = requests.get(ESP32_IP + "/api/v1/move")
        return res.json()
    except:
        return {"status": "error", "detail": "ESP32 no responde"}
