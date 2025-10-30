#include <WiFi.h>
#include <PubSubClient.h>

// ================= CONFIGURACIÓN MQTT Y WIFI =================
const char* ssid = "TuSSID";
const char* password = "TuPASSWORD";
const char* mqtt_server = "broker.hivemq.com";  // Puedes usar tu propio broker
const int mqtt_port = 1883;

// Tema MQTT específico para la simulación del sensor
const char* topic_sensor = "carro/sensor/distancia";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastSensorRead = 0;

// ================= CONEXIÓN A WIFI =================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
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
  client.publish(topic_sensor, msg);
  Serial.println("Distancia publicada en MQTT");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  randomSeed(analogRead(0)); // Inicializa aleatoriedad
}

// ================= LOOP =================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long ahora = millis();
  // Publicar cada 5 segundos
  if (ahora - lastSensorRead > 5000) {
    lastSensorRead = ahora;
    publicarDistancia();
  }
}
