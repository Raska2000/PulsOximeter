#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_HTU21DF.h"
#include "MAX30100_PulseOximeter.h"

Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#define REPORTING_PERIOD_MS     1000
#define OLED_RESET 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
double count=0;
double oxigen=0;




// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//DE LA PULSOXIMETRU


// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

 //Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("♥ Beat!");
}





void setup()   {                
  Serial.begin(115200);

 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);
   // Clear the buffer.
  display.clearDisplay(); 
  
  //DE LA TEMP SI UMIDITATE

  Serial.println("HTU21D-F test");

  if (!htu.begin()) {
    Serial.println("Couldn't find temp humidity sensor!");
   // while (1);//stops if failed
  }

  // DE LA PULSOXIMETRU
  Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        Serial.println("pulsxoximetru initialization FAILED");
        //for(;;); //stops if failed
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);

}


void loop() {
  float temp = htu.readTemperature();                                       //temp humidity sensor
    float rel_hum = htu.readHumidity();
    Serial.print("Temp: "); Serial.print(temp); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(rel_hum); Serial.println(" \%");
   // delay(50);//delay(500);

  // de la pulsoximetru

   pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        oxigen = pox.getSpO2();
        Serial.println("%");

        tsLastReport = millis();
    }


  String vString =  String(count, 1);// using a float and the             //display
  String OxiString = String(oxigen, 0 );
  display.clearDisplay();
  DrawText("Temp:          ", 4, 3, 1, false);
  DrawText(vString, 72, 3, 1, false);
  DrawText("C", 98, 3, 1, false);

  DrawText("Oxygen:    ", 4, 11, 1, false);
  DrawText(OxiString, 72, 11, 1, false);
  DrawText("%", 88, 11, 1, false);

  DrawText("BPM:   99", 4, 21, 1, false);
  //display.drawLine(1, 37, 100, 37, WHITE);
 display.drawRect(1, 1, 126,31, WHITE);
  //display.drawCircle(63,31, 31, WHITE);
   //display.startscrollright(0x00, 0x0F);
  display.display();
  count = temp;
   delay(10); 
}



void DrawText(String text, int x, int y,int size, boolean d) {

  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
  
  //delay(100);
}
