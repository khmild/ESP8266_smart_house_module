#include "SmartHouse.h"

void AHT10_setup(){
    while (AHT10_sens.begin(SDA, SCL) != true)
    {
        Serial.println("AHT10 not connected or fail to load calibration coefficient");
        delay(1000);
    }
}


void AHT10_read(){
    AHT10_temp = AHT10_sens.readTemperature(AHT10_FORCE_READ_DATA);
    AHT10_humid = AHT10_sens.readHumidity(AHT10_USE_READ_DATA);
}


void display_setup(){
    display.init();
    display.clear();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
}


void display_values(){
    
    int temp_int = (int)AHT10_temp;
    int temp_fr = (int)(AHT10_temp*10)-temp_int*10;
    int hum_int = (int)AHT10_humid;
    int hum_fr = (int)(AHT10_humid*10)-hum_int*10;
    
    display.clear();

    display.setFont(ArialMT_Plain_10);
    display.drawString(10,5,"TEMP C");
    display.drawString(73,5,"HUMID %");
    //-----------TEMP
    display.setFont(ArialMT_Plain_24);
    display.drawString(7,25, String(temp_int));
    display.drawString(32,25, ".");
    display.setFont(ArialMT_Plain_16);
    display.drawString(38,32, String(temp_fr));

    //-----------HUMID
    display.setFont(ArialMT_Plain_24);
    display.drawString(73,25, String(hum_int));
    display.drawString(99,25, ".");
    display.setFont(ArialMT_Plain_16);
    display.drawString(105,32, String(hum_fr));

    display.display();

}