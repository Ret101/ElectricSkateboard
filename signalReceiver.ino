#include <esp_now.h>
#include <WiFi.h>

//Structure example to receive data
//Must match the sender structure
typedef struct test_struct {
  int arr[5];
} test_struct;

//Create a struct_message called myData
test_struct myData;

uint8_t boardOutput = 26;
uint8_t direction = 33;
uint8_t brake = 35;
bool connected = false;
unsigned long timer = 0;

//callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info* mac, const uint8_t* incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.arr[0]);
  timer = millis();
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(9600);  
  pinMode(brake, OUTPUT);
  pinMode(direction, OUTPUT);
  digitalWrite(direction,LOW);
  digitalWrite(brake, HIGH);
  

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
} 
void loop() {  
  esp_now_register_recv_cb(OnDataRecv);
  digitalWrite(direction, LOW);
  digitalWrite(brake, HIGH);
  digitalWrite(direction,myData.arr[2]);
  if(timer < millis()-250){
    analogWrite(boardOutput,0);
    digitalWrite(brake, LOW);
  }else{
    digitalWrite(brake, 1 - myData.arr[0]);
    analogWrite(boardOutput,myData.arr[1]);
  }
}
