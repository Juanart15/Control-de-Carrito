// En este codigo se presenta el codigo funcional para la simulacion del sensor de ultrasonido, generando valores realistas de distancia y publicando estos en un tema MQTT independiente
// Se agrega el archivo config, para generar por separado las constantes y parametros de configuracion para el fucinomaiento del codigo
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"  // Archivo con configuraciones

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastSensorRead = 0;

// ================= CONEXIÓN A WIFI =================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("✅ WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// ================= CONEXIÓN A MQTT =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32_Carro")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5s");
      delay(5000);
    }
  }
}

// ================= FUNCIÓN DE SIMULACIÓN =================
float leerDistanciaSimulada() {
  // Simula una distancia entre 5 cm y 200 cm
  float distancia = random(5, 200) + random(0, 100) / 100.0;
  Serial.print("Distancia simulada: ");
  Serial.print(distancia);
  Serial.println(" cm");
  return distancia;
}

// ================= PUBLICACIÓN PERIÓDICA =================
void publicarDistancia() {
  float distancia = leerDistanciaSimulada();
  char msg[50];
  snprintf(msg, sizeof(msg), "{\"distancia\": %.2f}", distancia);
  client.publish(MQTT_TOPIC_SENSOR, msg);
  Serial.println("📡 Distancia publicada en MQTT");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  randomSeed(analogRead(0)); // Inicializa aleatoriedad
}

// ================= LOOP =================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long ahora = millis();
  if (ahora - lastSensorRead > SENSOR_INTERVAL_MS) {
    lastSensorRead = ahora;
    publicarDistancia();
  }
}
