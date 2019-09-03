#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
const char* ssid     = "SM|LY";
const char* password = "namakamu";

const char* host = "ta2018.web.id";
String indikco, indikno2, indikdebu;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();
  lcd.setCursor(0,0);
  lcd.print("Connecting to ");
  lcd.setCursor(0,1);
  lcd.println(ssid);
    Serial.print("Connecting to ");
  Serial.println(ssid);
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  lcd.setCursor(0,0);
  lcd.print("WiFi connected"); 
  delay(2000);
  Serial.println("WiFi connected");
  lcd.setCursor(0,0);
  lcd.println("Waiting connect ");  
  Serial.println("IP address: ");
  lcd.setCursor(0,1);
  lcd.println(" ta2018.web.id  ");
  Serial.println(WiFi.localIP());
     while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Open serial communications and wait for port to open:
 delay(2000);
}


void loop() {  
   // run over and over
  if (Serial.available()) {

    float L = Serial.parseFloat();
    float O = Serial.parseFloat();
    float C = Serial.parseFloat();
    float N = Serial.parseFloat();
    float D = Serial.parseFloat();
    Serial.write(Serial.read());
    lcd.setCursor(0,0);
    lcd.print("Send data to   ");
    Serial.print("Send data to ");
    lcd.setCursor(0,1);
    lcd.println(host);
  Serial.println(host);
     WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    lcd.setCursor(0,0);
    lcd.println("connection failed");
    Serial.println("connection failed");
    return;
  }
  //indikatorCO
    if(C<=2) 
    {
      indikco = "(Aman)";
    } 
    else 
    if (C>2)
    {
    indikco = "(Waspada)"; 
    }
    else 
    if (C>5)
    {
    indikco = "(Bahaya)"; 
    }

     //indikator NO2
    if(N<=0.02) 
    {
      indikno2 = "(Aman)";
    } 
    else 
    if (N>0.02)
    {
    indikno2 = "(Waspada)"; 
    }
    else 
    if (N>0.1)
    {
    indikno2 = "(Bahaya)"; 
    }

    //indikator Debu
    if(D<=0.15) 
    {
      indikdebu = "(Aman)";
    } 
    else 
    if (D<=0.3)
    {
    indikdebu = "(Waspada)"; 
    }
    else 
    if (D>0.3)
    {
    indikdebu = "(Bahaya)"; 
    }
  // We now create a URI for the request
  String url = "/insert1.php?lat=";
  url += String (L,6) ;
  url += "&long=";
  url += String (O,6) ;
  url += "&co=";
  url += C;
  url += "&no2=";
  url += N;
  url += "&debu=";
  url += D;
  url += "&inco=";
  url += indikco;
  url += "&inno2=";
  url += indikno2;
  url += "&indebu=";
  url += indikdebu;
  
  
  Serial.print("Req/ URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  lcd.setCursor(0,0);
  lcd.println("closing connection");
  Serial.println("closing connection");
 delay(5000);
  }
}
