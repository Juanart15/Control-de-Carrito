#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- CONFIGURACIÓN WiFi ---
const char* ssid = "";        // ← tu red WiFi
const char* password = "";    // ← tu contraseña

// --- CONFIGURACIÓN MQTT ---
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// --- SERVIDOR HTTP ---
WebServer server(80);

// --- CONFIGURACIÓN DE MOTORES ---
#define ENA 13   // PWM motor A
#define IN1 14   // Dirección motor A
#define IN2 27   // Dirección motor A
#define IN3 26   // Dirección motor B
#define IN4 25   // Dirección motor B
#define ENB 12   // PWM motor B

// --- FUNCIONES DE MOVIMIENTO ---
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// 🔁 Ajuste de mapeo: ahora cada comando se comporta correctamente
void moveForward(int speed) {
  // Avanzar hacia adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moveBackward(int speed) {
  // Retroceder
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  // Girar a la izquierda (una rueda avanza, la otra se detiene)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, speed);
}

void turnRight(int speed) {
  // Girar a la derecha
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, 0);
}

// --- CONEXIÓN WiFi ---
void setupWiFi() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// --- REINTENTO MQTT ---
void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP32CarClient")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

// --- ENDPOINT /status ---
void handleStatus() {
  String json = "{\"status\":\"online\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";
  server.send(200, "application/json", json);
}

// --- ENDPOINT /move ---
void handleMove() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"missing body\"}");
    return;
  }

  String body = server.arg("plain");
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "application/json", "{\"error\":\"invalid JSON\"}");
    return;
  }

  String direction = doc["direction"];
  int speed = doc["speed"];
  int duration = doc["duration"];
  if (duration > 5) duration = 5;  // límite de seguridad

  if (direction == "forward") moveForward(speed);
  else if (direction == "backward") moveBackward(speed);
  else if (direction == "left") turnLeft(speed);
  else if (direction == "right") turnRight(speed);
  else {
    server.send(400, "application/json", "{\"error\":\"invalid direction\"}");
    return;
  }

  // Publicar en MQTT
  String msg = "IP:" + WiFi.localIP().toString() +
               " | Dir:" + direction +
               " | Vel:" + String(speed) +
               " | Dur:" + String(duration);
  mqttClient.publish("car/move", msg.c_str());

  server.send(200, "application/json", "{\"status\":\"moving\"}");

  // Detener después de duración
  delay(duration * 1000);
  stopCar();
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  setupWiFi();
  mqttClient.setServer(mqtt_server, 1883);
  reconnectMQTT();

  server.on("/status", handleStatus);
  server.on("/move", HTTP_POST, handleMove);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
}
