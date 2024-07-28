#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t * esp_now_info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();

  // If you need to get the MAC address of the sender
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           esp_now_info->src_addr[0], esp_now_info->src_addr[1], esp_now_info->src_addr[2],
           esp_now_info->src_addr[3], esp_now_info->src_addr[4], esp_now_info->src_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

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

  Serial.print("ESP32 MAC Address: ");
Serial.println(WiFi.macAddress());

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing to do here
}