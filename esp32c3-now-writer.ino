#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(9600);

  // 等待串口准备就绪
  while (!Serial) {
    ; // 等待串口连接。注意：仅适用于原生 USB 端口的板子
  }

  Serial.println("ESP32-C3 is starting...");
  Serial.println("If you can see this, serial communication is working.");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // 设置 Wi-Fi 通道
  int channel = 1;
  WiFi.channel(channel);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for Send CB to get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);

  // 注册广播地址为对等设备
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = channel;  // 使用相同的通道
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  Serial.print("ESP32 MAC Address: ");
Serial.println(WiFi.macAddress());
}

void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  delay(2000);
}