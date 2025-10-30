#ifndef CONFIG_H
#define CONFIG_H

// ================= CONFIGURACIÓN WIFI =================
#define WIFI_SSID       "SSID"
#define WIFI_PASS       "PASSWORD"

// ================= CONFIGURACIÓN MQTT =================
#define MQTT_SERVER     "broker.hivemq.com"
#define MQTT_PORT       1883
#define MQTT_TOPIC_SENSOR "carro/sensor/distancia"

// ================= INTERVALO DE PUBLICACIÓN =================
#define SENSOR_INTERVAL_MS 5000  // Publicar cada 5 segundos

#endif
