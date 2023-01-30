float powerIN=0;
float powerOUT=0;
float r_Vin,r_Vout,r_Iin,r_Iout;
float efficiency=0;

uint8_t brightness = 0;
uint8_t modifier = 15;

void loop1(){
    //printPWMInfo(PWM_Instance);
    statusReport();
    powerCalculate();
    showValues();
    delay(100);
}

void showValues(){
    u8g2.clearBuffer();                   
    u8g2.setFont(u8g2_font_ncenB08_tr); 
    u8g2.drawStr(0,10,"Vi     Ii     Vo    Io");
    u8g2.setCursor(10,10);
    u8g2.print(r_Vin);
    u8g2.setCursor(30,10);
    u8g2.print(r_Iin);
    u8g2.setCursor(60,10);
    u8g2.print(r_Vout);
    u8g2.setCursor(90,10);
    u8g2.print(r_Iout);
    u8g2.drawStr(0,30,"Pi     Po      ");
    u8g2.setCursor(20,30);
    u8g2.print(powerIN);
    u8g2.setCursor(50,30);
    u8g2.print(powerOUT);
    u8g2.drawStr(0,50,"Eff     ");
    u8g2.setCursor(30,50);
    u8g2.print(efficiency);
    u8g2.sendBuffer();
}

void powerCalculate(){
    r_Vin=fmap(Vin,0,4096,0,38);
    r_Vout=fmap(Vout,0,4096,0,65);
    r_Iin=fmap(Iin,-1240,2865,-5,5);
    r_Iout=fmap(Iout,-1240,2865,-5,5);
    powerIN = r_Vin * r_Iin;
    powerOUT = r_Vout * r_Iout;
    efficiency = powerOUT / powerIN;
}

void statusReport(){
    pixels.clear();
    switch(state){
        case 0:
            brightness=255;
            pixels.setPixelColor(0,255,255,0);//giallo
            pixels.setBrightness(brightness);
            break;
        case 1:
            pixels.setPixelColor(0,255,255,0);//giallo
            dimming();
            break;
        case 2:
            pixels.setPixelColor(0,0,255,255);//azzurro
            dimming();
            break;
        case 3:
            pixels.setPixelColor(0,0,0,255);//blu
            dimming();
            break;
        case 4:
            brightness=255;
            pixels.setPixelColor(0,0,255,0);//verde
            pixels.setBrightness(brightness);
            break;
        case 5:
            pixels.setPixelColor(0,0,255,0);//verde
            dimming();
            break;
        case 10:
            brightness=255;
            pixels.setPixelColor(0,255,0,0);//rosso
            pixels.setBrightness(brightness);
            break;
        case 11:
            pixels.setPixelColor(0,255,0,0);//rosso
            dimming();
            break;
        case 12:
        case 13:
        case 14:
            pixels.setPixelColor(0,255,0,255);//rosa
            dimming();
            break;
    }
    pixels.show();
}

void dimming(){
    if(brightness == 255 || brightness == 0){
        modifier *= -1;
    }
    brightness += modifier;
    pixels.setBrightness(brightness);
}
