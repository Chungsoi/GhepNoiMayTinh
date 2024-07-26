#include <SPI.h>
#include <UIPEthernet.h>
EthernetClient client;
#include <string.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);
long next = 0,TimeCount = 0,count = 0;
boolean connectSever = false;
uint8_t mac[6] = {0x10,0xfe,0x02,0x03,0x04,0x05};
//=======================
void setup() { 
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("MyIP:");
  Ethernet.begin(mac);
  Ethernet.localIP();
  lcd.print(Ethernet.localIP());
  Ethernet.subnetMask();
  Ethernet.gatewayIP();
  Ethernet.dnsServerIP();
  Serial.begin(9600);
//  Serial.print("localIP: ");
//  Serial.println(Ethernet.localIP());
//  Serial.print("subnetMask: ");
//  Serial.println(Ethernet.subnetMask());
//  Serial.print("gatewayIP: ");
//  Serial.println(Ethernet.gatewayIP());
//  Serial.print("dnsServerIP: ");
//  Serial.println(Ethernet.dnsServerIP());
 for(int led = 5;led < 7; led++)
 {
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
 }
}

void loop() 
{
//============Check ket noi sever=================
if (millis()>= next +10)//5s check ket noi 1 lan
 {
    next = millis();
         
     if (connectSever == false)// IP và PORT
       {  Ethernet.begin(mac);
          Ethernet.localIP();
          Ethernet.subnetMask();
          Ethernet.gatewayIP();
          Ethernet.dnsServerIP();
          client.connect(IPAddress(192,168,1,3),1450);
          receiveDataServer();
          connectSever = true;
       }
     else
      {
     receiveDataServer();
      }
  }
 }
//=====================
void receiveDataServer()
{
  while (connectSever == true)
    {
        int size;
        while((size = client.available()) > 0)
        {
          char dataReceive[4]= {}; // bien luu gia tri de^m.
          size = client.read(dataReceive,size);
           Serial.println(dataReceive);
          switch (dataReceive[0])
          {
            case'@': 
            Serial.println("======Stat Read======");
            switch (dataReceive[2])
            {
              case'$':
              Serial.print("conmand = ");
              Serial.println(dataReceive[1]);
              Serial.println("=========End Read==========");
              switch (dataReceive[1])
              {
                case '0':
                digitalWrite(5,1);
                break;

                case '1':
                digitalWrite(5,0);
                break;

                case '2':
                digitalWrite(6,0);
                break;

                case '3':
                digitalWrite(6,1);
                break;

                case '4':
                digitalWrite(5,0);
                digitalWrite(6,0);
                break;

                case '5':
                digitalWrite(5,1);
                digitalWrite(6,1);
                break;
              }
              break; 
            }
            break;
          }
        }
        
      //==========Khoi lenh tu ket noi lai server=============
        while((size = client.available())  <= 0)
        {
          //Sau 10s k co Du lieu tra ve thi tu dong connect lai server
          if(millis()>TimeCount + 1000)
            {
              TimeCount = millis();
              count++;
             // Serial.println(count);
              if(count > 30)
              {
                count = 0;
                connectSever = false;
              }
            }
        }
        count = 0;
          //=======================
     }
}
