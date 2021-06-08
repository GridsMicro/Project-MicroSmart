# Project-MicroSmart
Project : IoT By Microtronic.biz

## NodeMCU ESP8266 12E

เรียกใช้งาน library 

```C++
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
```

กำหนดค่าการเชื่อมต่อ wifi

```C++
const char* ssid = "admin"; 
const char* password =  "12345678";
```

กำหนดค่าเซิร์ฟเวอร์ MQTT เราใช้เซิร์ฟเวอร์ Eclipse MQTT สำหรับโครงการนี้ ซึ่งเป็นสาเหตุที่ทำให้ที่อยู่เซิร์ฟเวอร์เป็น“mqtt.eclipse.org” แต่ถ้าคุณวางแผนที่จะใช้เซิร์ฟเวอร์อื่น เช่น Mosquitto, Adafruit คุณสามารถแทนที่ด้วยที่อยู่เซิร์ฟเวอร์และหมายเลขพอร์ตเฉพาะของคุณ

```C++
const char* mqtt_server = "mqtt.eclipse.org"; 
const int mqtt_port = 1883;
```

ถัดไป อินสแตนซ์จะถูกสร้างขึ้นสำหรับคลาส `WiFiClient` และ `PubSubClient`

```C++
WiFiClient espClient;
PubSubClient client(espClient);
```

ในส่วน `setup()` เราเรียก `WiFi.begin()` ก่อน การเรียกวิธีนี้จะเชื่อมต่อ ESP กับ HotSpot ที่คุณต้องการ

```C++
WiFi.begin(ssid, password);
```

ตรวจสอบการเชื่อมต่อเครือข่ายโดยใช้  `WiFi.status ()` หลังจากการเชื่อมต่อสำเร็จ ข้อความจะถูกพิมพ์บน Serial Monitor พร้อม SSID ของเครือข่ายที่เชื่อมต่อ

```C++
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
```

สร้างโบรกเกอร์ เราได้ใช้เมธอด `setServer` วิธีนี้ใช้สองอาร์กิวเมนต์ที่เราได้กำหนดไว้ล่วงหน้าแล้ว ตอนนี้ ถ้าเราต้องการรับข้อความจากเซิร์ฟเวอร์ เราต้องสร้างฟังก์ชันเรียกกลับ เพื่อที่เราจะใช้ `setCallback (callback)` เมธอด

```C++
client.setServer(mqtt_server, mqtt_port);
client.setCallback(MQTTcallback);
```

ตอนนี้ เราได้ใช้ฟังก์ชัน `connect (clientID`) เพื่อเชื่อมต่อกับไคลเอนต์ ESP8266 

**NOTE** : `clientID` คือชื่ออุปกรณ์ต้องไม่ซ้ำกัน หากเชื่อมต่อแล้ว ข้อความแสดงความสำเร็จสามารถแสดงได้ภายในมอนิเตอร์แบบอนุกรม

```C++
if (client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ")
      Serial.println(client.state());
      delay(2000);
    }
```

เรียก `client.subscribe()` ซึ่งเป็นฟังก์ชัน MQTT ในตัว ซึ่งใช้ในการ subscribe จากหัวข้อเฉพาะ (สำหรับโครงการนี้ เราใช้ “`esp/test`” เป็นชื่อสมาชิกของเรา)

```C++
client.subscribe("esp/test");
```

ขณะนี้ฟังก์ชัน `MQTTcallback` ถูกเรียกเพื่อตรวจสอบว่ามีข้อมูลที่อัปเดตหรือไม่ หากมีข้อมูลใหม่ ฟังก์ชันนี้จะจัดการข้อมูลที่ได้รับและพิมพ์ข้อความใน Serial Monitor พร้อมข้อความต้นฉบับและชื่อหัวข้อที่ได้รับข้อความมา

ต่อไป เราต้องแปลงข้อความเป็นสตริง เพื่อให้สามารถเปรียบเทียบและตรวจสอบการกระทำที่ทริกเกอร์ได้ ในโครงการนี้ ไฟ LED จะถูกเปิด/ปิดโดยใช้คำสั่ง MQTT ดังแสดงในโค้ดด้านล่าง

```C++
for (int i = 0; i < length; i++)
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  if (message == "on")
  {
    digitalWrite(LED, HIGH);
  }
```

เพื่อเผยแพร่ข้อมูลในหัวข้อ ฟังก์ชั่น `client.publish()` จึงถูกนำมาใช้ ในโปรเจ็กต์นี้ 

สถานะปุ่มกดจะถูกตรวจสอบ หากกดปุ่ม ข้อความจะถูกเผยแพร่ไปยังหัวข้อ “esp/test1” ดังที่แสดงด้านล่าง

```C++
if(digitalRead(D1)==0))
  {
  client.publish("esp/test1", "Hello from ESP8266");
  }
  else;
client.loop();
```

### สำหรับการทดสอบครั้งสุดท้าย เราจะใช้แอปพลิเคชัน Android ซึ่งเราได้ตั้งค่าไว้ก่อนหน้านี้
- เปิดแอปพลิเคชันไคลเอนต์ MQTT และตรวจสอบให้แน่ใจว่ามือถือของคุณมีการเชื่อมต่ออินเทอร์เน็ตที่ใช้งานได้ นอกจากนี้ ฮอตสปอตที่เชื่อมต่อ NodeMCU ควรมีการเชื่อมต่ออินเทอร์เน็ตที่ใช้งานได้ 
- เมื่อทุกอย่างเชื่อมต่อกับอินเทอร์เน็ตแล้ว เราจะส่งสตริง "Hello from ESP8266" จากโมดูล ESP ซึ่งจะปรากฏในแอป Android และเราจะได้รับการแจ้งเตือน 
- ต่อไป เราจะส่งสตริงจากแอป Android ซึ่งจะเปิด LED ที่เชื่อมต่อกับบอร์ด ESP8266 Node MCU