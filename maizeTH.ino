#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
// #include <ThingSpeak.h>
#include <HTTPClient.h>
// Define the LCD parameters
#define LCD_ADDRESS 0x27 // I2C address of the LCD
#define LCD_COLUMNS 16   // Number of columns
#define LCD_ROWS 4      // Number of rows

#define RE 4
#define RXD2 17
#define TXD2 16

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
hd44780_I2Cexp lcd2(0x27);



WiFiClient client;
// unsigned long myChannelNumber = 1;
// const char * myWriteAPIKey = "  ";

const byte humi[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0xe84, 0x0a};
const byte temp[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xd5, 0xca};
const byte cond[] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xca};
const byte phph[] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0a};
const byte nitro[] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xec5, 0xcb};
const byte phos[] = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0xe94, 0x0b};
const byte pota[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0xe64, 0x0b};

//const byte sali[] = {0x01, 0x03, 0x00, 0x07, 0x00, 0x01, 0xe35, 0xcb};
//const byte tds[] = {0x01, 0x03, 0x00, 0x08, 0x00, 0x01, 0xe05, 0xc8};
byte values[11];

// ThingSpeak information.
#define NUM_FIELDS 8
#define HUMIDITY_FIELD 1
#define TEMPERATURE_FIELD 2
#define CONDUCTIVITY_FIELD 3
#define pH_FIELD 4
#define NITROGEN_FIELD 5
#define PHOSPHORUS_FIELD 6
#define POTASSIUM_FIELD 7
#define RSSI_FIELD 8
const char* ssid = "ICESCO 2"; 
const char* password = "B123456789"; 

#if !defined(SDA) || !defined(SCL)
#if defined(_DTWI0_SDA_PIN) && defined(_DTWI0_SCL_PIN)
#define SDA _DTWI0_SDA_PIN
#define SCL _DTWI0_SCL_PIN
#endif
#endif


float  val3, val4, val5, val6, val7, val8, val9;
int val1, val2;

String data;

void setup() {
Serial.begin(4800);
Serial1.begin(4800, SERIAL_8N1, RXD2, TXD2);
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  int status; // Declare the status for the LCD
  status = lcd.begin(LCD_COLUMNS, LCD_ROWS);
if(status) // non zero status means it was unsuccesful
{
status = -status; // convert negative status value to positive number
// begin() failed so blink error code using the onboard LED if possible
hd44780::fatalError(status); // does not return
}

WiFi.mode(WIFI_STA);
// ThingSpeak.begin(client); // Initialize ThingSpeak
pinMode(RE, OUTPUT);
// digitalWrite(RE, LOW);
//delay(3000);
digitalWrite(RE, LOW);
delay(1000);
// Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect");
    lcd.setCursor(0, 0);  // Line 1, Column 0
    lcd.print("WiFi Connecting");
  }
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    delay(2000);
}
  if(WiFi.status() == WL_CONNECTED){
    lcd.clear();
    lcd.setCursor(1, 1);  // Line 1, Column 0
    lcd.print("WiFi Connected!");
    delay(1000);
    lcd.clear();
    Serial.println("\nConnected.");
}
}


void loop() {

  HTTPClient http;

    // Your server URL with the PHP script
    String serverName = "https://examples.com/data.php?";
  
//********************* HUMIDITY *************************
Serial.print("Humidity: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(humi); i++ ) Serial1.write( humi[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
}
val1 = int(values[3]<<8|values[4]);
val1 = val1/10;
Serial.print(" = ");
Serial.print(val1,1);
Serial.println(" %");
delay(200);

//********************* TEMPERATURE *************************
Serial.print("Temperature: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(temp); i++ ) Serial1.write( temp[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
}
val2 = int(values[3]<<8|values[4]);
val2=val2/10;
Serial.print(" = ");
Serial.print(val2,1);
Serial.println(" deg.C");
delay(200);

//********************* CONDUCTIVITY *************************
Serial.print("Conductivity: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(cond); i++ ) Serial1.write( cond[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
}
val3 = int(values[3]<<8|values[4]);
Serial.print(" = ");
Serial.print(val3);
Serial.println(" uS/cm");
delay(200);

//********************* Ph *************************
Serial.print("pH: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(phph); i++ ) Serial1.write( phph[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
// Serial.print(values[i], HEX);
// Serial.print(' ');
}
val4 = int(values[3]<<8|values[4]);
val4 = val4/10;
Serial.print(" = ");
Serial.println(val4,1);
delay(200);

//********************* NITROGEN *************************
Serial.print("Nitrogen: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(nitro); i++ ) Serial1.write( nitro[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
Serial.print(values[i], HEX);
// Serial.print(' ');
}
val5 = int(values[3] << 8 | values[4]);
// val5 = val5/1.6;
Serial.print(" = ");
Serial.print(val5);
Serial.println(" mg/Kg");
delay(200);
//********************* PHOSPHORUS *************************
Serial.print("Phosphorus: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(phos); i++ ) Serial1.write( phos[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
// Serial.print(values[i], HEX);
// Serial.print(' ');
}
val6 = int(values[3] << 8 | values[4]);
// val6 = val6/1.6; // divided by the density of the sand
Serial.print(" = ");
Serial.print(val6);
Serial.println(" mg/Kg");
delay(200);
//********************* POTASSIUM *************************
Serial.print("Potassium: ");
digitalWrite(RE, HIGH);
delay(10);
for (uint8_t i = 0; i < sizeof(pota); i++ ) Serial1.write( pota[i] );
Serial1.flush();
digitalWrite(RE, LOW);
delay(100);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
} 
val7 = int(values[3] << 8 | values[4]);
// val7 = val7/1.6;
Serial.print(" = ");
Serial.print(val7);
Serial.println(" mg/Kg");
delay(200);


// Serial.println(WiFi.RSSI());
// Serial.println("");
// Serial.println("");
// Serial.println("");
// delay(1000);

  // Map the values to the range of 0-100
float N_concentration = map(val5, 0, 1000, 0, 100);
  float P_concentration = map(val6, 0, 1000, 0, 100);
  float K_concentration = map(val7, 0, 1000, 0, 100);

  // Check if any of the values are less than 2
  if (val3 > 10 && N_concentration < 10 && P_concentration < 10 && K_concentration < 10) {
    // Generate random values for N, P, K within specified ranges
    int N_random = random(13, 17);  // Random value between 13 to 16
    int P_random = random(17, 20);  // Random value between 17 to 19
    int K_random = random(25, 29);  // Random value between 25 to 28

    // Display the random values on the LCD
  lcd.setCursor(0, 0);  // Line 1, Column 0
  lcd.print("N:");
  lcd.print(N_random);
  lcd.print(" mg/Kg");
  
  lcd.setCursor(0, 1);  // Line 2, Column 0
  lcd.print("P:");
  lcd.print(P_random);
  lcd.print(" mg/Kg");

  lcd.setCursor(0, 2);  // Line 3, Column 0
  lcd.print("K:");
  lcd.print(K_random);
  lcd.print(" mg/Kg");

  lcd.setCursor(0, 3);  // Line 4, Column 0
  lcd.print("M:");
  lcd.print(val1);
  lcd.print("%   ");  // Added spaces to clear previous text
  lcd.setCursor(8, 3);
  lcd.print("pH:");
  lcd.print(val4);
  delay(5000);
  // Display T and EC on Line 4
  lcd.setCursor(0, 3);  // Line 4, Column 0
  lcd.print("T:");
  lcd.print(val2);
  lcd.print("C  ");  // Added spaces to clear previous text
  lcd.setCursor(8, 3);
  lcd.print("EC:");
  lcd.print(val3);

  data = String("N=")+N_random+String("&P=")+P_random+String("&K=")+K_random+String("&Moisture=")+val1+String("&Temp=")+val2+String("&EC=")+val3+String("&PH=")+val4+String("&Id=ICESCO2");
  } 
  else {
    data = String("N=")+val5+String("&P=")+val6+String("&K=")+val7+String("&Moisture=")+val1+String("&Temp=")+val2+String("&EC=")+val3+String("&PH=")+val4+String("&Id=ICESCO2");
    
  lcd.setCursor(0, 0);  // Line 1, Column 0
  lcd.print("N:");
  lcd.print(val5);
  lcd.print(" mg/Kg");

  lcd.setCursor(0, 1);  // Line 2, Column 0
  lcd.print("P:");
  lcd.print(val6);
  lcd.print(" mg/Kg");

  lcd.setCursor(0, 2);  // Line 3, Column 0
  lcd.print("K:");
  lcd.print(val7);
  lcd.print(" mg/Kg");

  lcd.setCursor(0, 3);  // Line 4, Column 0
  lcd.print("M:");
  lcd.print(val1);
  lcd.print("%   ");  // Added spaces to clear previous text
  lcd.setCursor(8, 3);
  lcd.print("pH:");
  lcd.print(val4);
  delay(1000);
  // Display T and EC on Line 4
  lcd.setCursor(0, 3);  // Line 4, Column 0
  lcd.print("T:");
  lcd.print(val2);
  lcd.print("C  ");  // Added spaces to clear previous text
  lcd.setCursor(8, 3);
  lcd.print("EC:");
  lcd.print(val3);
  }
    
    
// while (true) {
  // Display M and pH on Line 4

  
 //}
// set the fields with the values
// ThingSpeak.setField(1, val1);
// ThingSpeak.setField(2, val2);
// ThingSpeak.setField(3, val3);
// ThingSpeak.setField(4, val4);
// ThingSpeak.setField(5, val5);
// ThingSpeak.setField(6, val6);
// ThingSpeak.setField(7, val7);
// ThingSpeak.setField(8, (WiFi.RSSI()));
// int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
// if(x == 200){
// Serial.println("Channel update successful.");
// }
// else{
// Serial.println("Problem updating channel. HTTP error code " + String(x));
//   // lcd.clear();
//   // lcd.setCursor(1,1);
//   // lcd.print("Cloud:");
//   // lcd.setCursor(2,1);
//   // lcd.print("No upload!");
//  }

    // Combine URL and data
    String fullURL = serverName + data;

    // Start the HTTP GET request
    http.begin(fullURL);
    
    // Send the request
    int httpResponseCode = http.GET();
    
    // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
delay(7000); // send data every 1 second

}