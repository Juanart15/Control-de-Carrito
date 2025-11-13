// Codigo completo que combina el todos los requisitos anteriores, añadiendo el funcionamiento del sensor de ultrasonido
// Tambien se anexa el archivo config.h para separar los constantes y parametros de configuracion
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include "config.h"

// ==========================
// Objetos principales
// ==========================

//Modificar el código de conexión a MQTT para usar un puerto seguro. Sin hacer más cambios verificar que la conexión sigue  funcionando (¿o no?). 
WiFiClientSecure espClient; //Realizado
PubSubClient mqttClient(espClient);
WebServer server(http_port);

// ==========================
// Prototipos
// ==========================
void conectarWiFi();
void conectarMQTT();
void manejarMovimiento();
void manejarStatus();
void moverCarro(String direccion, int velocidad, int duracion);
void simularYPublicarDistancia();
float simularLecturaDistancia();

// ==========================
// Variables globales
// ==========================
unsigned long lastSensorPublish = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  conectarWiFi();
  mqttClient.setServer(mqtt_server, mqtt_port);

  conectarMQTT();

  // ==========================
  // Configuración de Endpoints HTTP
  // ==========================
  server.on("/move", HTTP_POST, manejarMovimiento);
  server.on("/status", HTTP_GET, manejarStatus);
  server.begin();

  Serial.println("Servidor HTTP iniciado");
  Serial.print("🌐 IP local: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  mqttClient.loop();
  server.handleClient();

  // Publicación periódica del sensor simulado
  if (millis() - lastSensorPublish > 5000) { // cada 5 s
    simularYPublicarDistancia();
    lastSensorPublish = millis();
  }
}

// ==========================
// Funciones principales
// ==========================
void conectarWiFi() {
  Serial.print("Conectando a WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado ");
}

void conectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Conectando a MQTT...");
    if (mqttClient.connect("ESP32CarClient")) {
      Serial.println("MQTT conectado");
      mqttClient.subscribe(mqtt_topic_mov);
    } else {
      Serial.print("Error MQTT: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

// ==========================
// Endpoint /move
// ==========================
void manejarMovimiento() {
  if (!server.hasArg("direccion") || !server.hasArg("velocidad") || !server.hasArg("duracion")) {
    server.send(400, "application/json", "{\"error\": \"Faltan parámetros (direccion, velocidad, duracion)\"}");
    return;
  }

  String direccion = server.arg("direccion");
  int velocidad = server.arg("velocidad").toInt();
  int duracion = server.arg("duracion").toInt();

  if (duracion > 5000) duracion = 5000; // seguridad: máx 5s

  moverCarro(direccion, velocidad, duracion);

  // Publicar en MQTT
  String payload = "{\"ip_cliente\": \"" + WiFi.localIP().toString() + "\", \"direccion\": \"" + direccion +
                   "\", \"velocidad\": " + String(velocidad) + ", \"duracion\": " + String(duracion) + "}";
  mqttClient.publish(mqtt_topic_mov, payload.c_str());

  server.send(200, "application/json", "{\"status\": \"ok\", \"accion\": \"" + direccion + "\"}");
  Serial.println("Comando HTTP procesado y publicado en MQTT ");
}

// ==========================
// Endpoint /status
// ==========================
void manejarStatus() {
  server.send(200, "application/json", "{\"status\": \"online\", \"ip\": \"" + WiFi.localIP().toString() + "\"}");
}

// ==========================
// Movimiento del carro
// ==========================
void moverCarro(String direccion, int velocidad, int duracion) {
  Serial.println("Moviendo " + direccion + " | Velocidad: " + String(velocidad) + " | Duración: " + String(duracion));
  // Aquí irían las funciones digitales para controlar los motores
  delay(duracion);
  Serial.println("Movimiento detenido");
}

// ==========================
// Simulación del sensor HC-SR04
// ==========================
float simularLecturaDistancia() {
  float distancia = random(5, 100) + random(0, 99) / 100.0;
  Serial.print("📏 Distancia simulada: ");
  Serial.print(distancia);
  Serial.println(" cm");
  return distancia;
}

void simularYPublicarDistancia() {
  float distancia = simularLecturaDistancia();
  String payload = "{\"distancia_cm\": " + String(distancia, 2) + "}";
  if (mqttClient.publish(mqtt_topic_sensor, payload.c_str())) {
    Serial.println("📡 Distancia publicada en MQTT correctamente");
  } else {
    Serial.println(" Error al publicar distancia");
  }
}
