/**
 * Helloworld style, connect an ESP32 to IBM's Watson IoT Platform
 * 
 * Author: Mondragon
 * 1 sensor funcionando em delay de 10 segundos !!!Sempre abrir firewall para o novo ip senao fica bloqueado (ver log no painel da IBM IoT) 
 * 
 */
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Substitua pelo SSID da sua rede
#define SSID "YOUR_WIFI_SSID"

//Substitua pela senha da sua rede
#define PASSWORD "YOUR PASSWORD"

#define ORG "YOU_ORGANIZATION" // your organization or "quickstart"
#define DEVICE_TYPE "ESP32" // use this default for quickstart or customize to your registered device type
#define DEVICE_ID "YOUR_DEVICE_ID" // use this default for quickstart or customize to your registered device id
#define TOKEN "YOUR_TOKEN" // your device token or not used with "quickstart" IT IS GENERATED IN THE IBM PANEL IOT
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

//Variável para guardarmos o valor da pressao
float pressure1 = 0; 
 

int fsrPin2  = 2;     // the FSR and 10K pulldown are connected to a2


void setup() {
 Serial.begin(115200); delay(1); Serial.println();

  //Conectamos à rede WiFi
  setupWiFi();
}

void loop() {

 if (!!!client.connected()) {
   Serial.print("Reconnecting client to "); Serial.println(server);
   while (!!!client.connect(clientId, authMethod, token)) {
     Serial.print(".");
     delay(500);
   }
   Serial.println();
 }

  readSensor();

  //String payload = String(pressure);

 String payload = "{ \"DEVICE_ID\" : {\"Pressure1\":";
 payload += pressure1;
 payload += "}}";
 
 Serial.print("Sending payload: "); Serial.println(payload);
 
 if (client.publish(topic, (char*) payload.c_str())) {
   Serial.println("Publish ok");
 } else {
   Serial.println("Publish failed");
 }

 delay(10000);
}

//Função responsável por conectar à rede WiFi
void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);

  //Manda o esp se conectar à rede através
  //do ssid e senha
  WiFi.begin(SSID, PASSWORD);

  //Espera até que a conexão com a rede seja estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Se chegou aqui é porque conectou
  Serial.println("");
  Serial.println("WiFi connected");
}

//Função responsável por realizar a leitura 
//da temperatura e umidade
void readSensor(){
  float value2;

  
  //Faz a leitura da temperatura

  value2 = analogRead(fsrPin2);  

  //Se o valor lido é válido
  if(!isnan(value2)){  
    //Armazena o novo valor da temperatura
    pressure1 = value2;
 
  }
}
