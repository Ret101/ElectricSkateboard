#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress1[] = {0x08, 0xA6, 0xF7, 0xBC, 0x6B, 0x68};
bool connected = false;
uint8_t input = 33; 
uint8_t value = 30;
uint8_t loopnumber = 0;
typedef struct test_struct {
  int arr[5];
} test_struct;

test_struct test;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  connected = (status == ESP_NOW_SEND_SUCCESS);
}
 
void setup() {
  Serial.begin(9600); 
  WiFi.mode(WIFI_STA);
  pinMode(input,INPUT);

  while (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
esp_now_peer_info_t peerInfo;

  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  
   while (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } 
}
 
void loop() {     
  if(loopnumber == 50){
    loopnumber = 0;
    value = value + 5;
  }
  if(value >= 100){
    value = 100;
  }
  test.arr[1] = value;
  Serial.println(analogRead(input));
  esp_now_register_send_cb(OnDataSent);
  esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
  delay(10);
  loopnumber = loopnumber+1;
}