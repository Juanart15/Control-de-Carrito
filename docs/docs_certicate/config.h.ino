#ifndef CONFIG_H
#define CONFIG_H

// ==========================
// Pines de los motores
// ==========================
#define ENA 14
#define ENB 27
#define IN1 26
#define IN2 25
#define IN3 33
#define IN4 32

// ==========================
// Pines del sensor (simulado o físico)
// ==========================
#define TRIG_PIN 12
#define ECHO_PIN 13

// ==========================
// Configuración WiFi
// ==========================
const char* ssid = "SSID";
const char* password = "Password";

// ==========================
// Configuración MQTT
// ==========================
const char* mqtt_server = "broker.hivemq.com";

// Modificar el código de conexión a MQTT para usar un puerto seguro. Sin hacer más cambios verificar que la conexión sigue  funcionando (¿o no?). 
const int mqtt_port = 8883; // Realizado

// Temas MQTT
const char* mqtt_topic_mov = "carro/movimiento";
const char* mqtt_topic_sensor = "carro/distancia";

// ==========================
// Configuración HTTP
// ==========================
const int http_port = 80;

#endif
