#include <Wire.h>
#include "rgb_lcd.h"  // Grove LCD backlight库
#include <WiFiNINA.h>
#include <ThingSpeak.h>

char ssid[] = "OnePlus 9";
char pass[] = "e6vmhcy5";
char thingSpeakAPIKey[] = "KWAZ8WRCI0TNDREB";
int yourChannelNumber =  2398825;

WiFiClient client;
rgb_lcd lcd;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // 初始化LCD
  lcd.setRGB(0, 255, 0);  // 设置LCD背光颜色为绿色

  // 连接WiFi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("连接中...");
    delay(5000);
  }

  Serial.println("连接成功！");
}

void loop() {
  // 测量网络速度
  long speed = WiFi.RSSI();

  // 时延测量
  long pingTime = ping("www.google.com");

  // 将数据发送到ThingSpeak
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, speed);
  ThingSpeak.setField(2, pingTime);

  int status = ThingSpeak.writeFields(yourChannelNumber, thingSpeakAPIKey);

  if (status == 200) {
    Serial.println("数据发送成功！");
  } else {
    Serial.println("数据发送失败。");
  }

  // 在LCD上显示当前数据
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(speed);
  lcd.print(" dBm");

  lcd.setCursor(0, 1);
  lcd.print("Ping: ");
  lcd.print(pingTime);
  lcd.print(" ms");

  delay(15000);  // 每隔15秒发送一次数据并更新LCD
}

long ping(const char *host) {
  long pingTime;
  int packetSize;

  packetSize = WiFi.ping(host);

  if (packetSize > 0) {
    pingTime = WiFi.ping(host);
  } else {
    pingTime = -1;  // 如果ping失败，返回-1
  }

  return pingTime;
}
