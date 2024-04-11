#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

#define POS_INICIAL 90
#define X_MINIMO    30      // Angulo de X de 30 a 150 servo da base  -- PAN
#define Y_MINIMO    30      // Angulo de Y de 30 a 90 servo de cima   -- TILT
#define X_INTERVALO 12      // = (X Maximo - X Minimo) / 10
#define Y_INTERVALO 6       // = (Y Maximo - Y Minimo) / 10
#define LASER_TEMPO 60      // tempo em segundos que o laser ficará acionado
#define LASER       D1
#define PIR         D2      // sensor de presença
#define SERVO1      D3
#define SERVO2      D4

Servo servoX;
Servo servoY;

// --- SETUP WIFI E MQTT ---
const char* ssid = "...";
const char* password = "...";
const char* mqtt_server = "mqtt.eclipseprojects.io";
const int mqtt_port = 1883;
const char* mqtt_topic_status = "monitoracao/lasercat/status";
const char* mqtt_user = "...";
const char* mqtt_pwd = "...";
const char* mqtt_topic_ctrl = "monitoracao/lasercat/ctrl";

const char* statusOn = "ON";
const char* statusOff = "OFF";
bool statusCtrl = true;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  posicaoInicialServos();
  configuracaoInicialLaser();
  configuracaoInicialPIR();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(monitoraTopicoCtrl);
}

void posicaoInicialServos() {
  servoX.attach(SERVO1);
  servoY.attach(SERVO2);
  servoX.write(POS_INICIAL);
  servoY.write(POS_INICIAL);
}

void configuracaoInicialLaser() {
  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, LOW);
}

void configuracaoInicialPIR() {
  pinMode(PIR, INPUT);
}

void loop() {
  // --- Testa conexão do Wifi ---
  if (!client.connected()) {
    reconnect();
  }

  if (statusCtrlOff()) {  // funções desligadas via MQTT
    return;
  }

  int trigger = digitalRead(PIR);
  client.publish(mqtt_topic_status, trigger == HIGH ? statusOn : statusOff, true);    // true lê o ultimo status

  if (trigger == HIGH) {
    ligaLaser();
  } else {
    desligaLaser();
    delay(1000);
  }
}

// FUNCOES AUXILIARES

void posicionaServos() {
   int posicaoX = (random(0, (X_INTERVALO)) * 10 + (X_MINIMO));
   int posicaoY = (random(0, (Y_INTERVALO)) * 10 + (Y_MINIMO));
   servoX.write(posicaoX);
   servoY.write(posicaoY);
}

void ligaLaser() {
  for(int i = 0; i <= LASER_TEMPO / 2; i++) {
    digitalWrite(LASER, HIGH);
    posicionaServos();
    delay(2000);    // funciona melhor do que 1s

    if (statusCtrlOff()) {  // funções desligadas via MQTT
      return;
    }
  }
}

void desligaLaser() {
  digitalWrite(LASER, LOW);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_ctrl);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

// --- MONITORA TOPICO monitoracao/lasercat/ctrl ---
void monitoraTopicoCtrl(char* mqtt_topic_ctrl, byte* payload, unsigned int length) {
  if ((char)payload[0] == '1') {
    statusCtrl = 1;
    //Serial.println("statusCtrl ON");
  } else {
    statusCtrl = 0;
    //Serial.println("statusCtrl OFF");
  }
}

bool statusCtrlOff() {
    client.loop();
    bool statusJustTurnedOff = (statusCtrl == 0);
    if (statusJustTurnedOff) {
      client.publish(mqtt_topic_status, statusOff, true);    // true lê o ultimo status
      desligaLaser();
    }
    return statusJustTurnedOff;
}