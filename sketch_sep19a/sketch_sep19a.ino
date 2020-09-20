#include <WiFi.h> //Connect to WiFi Network
#include <string.h>
#include <mpu9255_esp32.h>
#include <Wire.h>
#include <WebSocketsServer.h>
#include <SPI.h>
//#include <MPU9250_asukiaaa.h>
char host[] = "game.vegetableassass.in";
char path[] = "/00000/";
MPU9255 imu;//Initializes imu object
int state = 0;
WebSocketsServer webSocket = WebSocketsServer(80);

const int MPU_addr=0x68;
const uint8_t input_pin1 = 14;
const char* ssid = "TOBAGH";
const char* password = "KS20FENWAY";
float x,y,z;
int timer;
float init_z,post_z;
void setup_imu(){
  if (imu.setupIMU(1)){
    Serial.println("IMU Connected!");
  } else{
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }
}

void setup_wifi(){
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());
}

// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    // Echo text message back to client
    case WStype_TEXT:
      Serial.printf("[%u] Text: %s\n", num, payload);
      webSocket.sendTXT(num, payload);
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //begin serial
  delay(50); //pause to make sure comms get set up
  Wire.begin();
  //mySensor.setWire(&Wire);
  //mySensor.beginAccel();
  //mySensor.beginMag();
  setup_imu();
  setup_wifi();
  pinMode(input_pin1, INPUT_PULLUP); //set input pin as an input!
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}
void get_imu_data(){
//  mySensor.accelUpdate();
imu.readAccelData(imu.accelCount);
  x = imu.accelCount[0]*imu.aRes;
  y = imu.accelCount[1]*imu.aRes;
  z = imu.accelCount[2]*imu.aRes;

 char output[40];
  //sprintf(output,"%4.2f,%4.2f,%4.2f",x,y,z); //render numbers with %4.2 float formatting
  //Serial.println(output); //print to serial for plotting

}
void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  get_imu_data();
  int buttonState = digitalRead(input_pin1);
  switch(state){
    case 0:
      if (buttonState == LOW){
        imu.readAccelData(imu.accelCount);
        init_z = imu.accelCount[2]*imu.aRes;
        timer = millis();
        Serial.print("pressed");
        state = 1;
      }
    break;
    case 1: 
      if (buttonState == HIGH){
        imu.readAccelData(imu.accelCount);
        post_z = imu.accelCount[2]*imu.aRes;
        Serial.println(init_z);
        Serial.println(post_z);
        float average = ((init_z-post_z)/(timer/1000));
        String a = String(average);
        webSocket.sendTXT(0,a);
        state = 0;
      }
    break;
  }
}
