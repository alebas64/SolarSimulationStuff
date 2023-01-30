#include "RP2040_PWM.h"
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>

#define pinLed 25    //BUILTIN_LED
#define analog_A0 26 //tensione in ingresso
#define analog_A1 27 //tensione in uscita
#define analog_A2 28 //corrente in ingresso
#define analog_A3 29 //corrente in uscita
#define digital_D6  0 //PWM channel 
#define digital_D10 3 //shutdown IR2104

#define neoPixel 11 //utilizzabile per debug visivo
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, neoPixel, NEO_GRB + NEO_KHZ800);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
 
#define VOUT_TARGET 40.0f
#define EPSELON 0.1f
#define EPSELON_CURRENT 0.1f
#define EPSELON_VOLTAGE 0.1f
#define DUTY_VARIATION 0.01f //variazione di 0.01% alla volta

RP2040_PWM* PWM_Instance;

float frequency = 250000.0f; //frequenza di 250kHz 
float dutyCycle = 0.0f;
//non c'è bisogno di utilizzare un valore in float, il microcontrollore
//è più veloce a fare conti con gli interi
volatile uint16_t Vin=0;
volatile uint16_t Vout=0;
volatile int16_t Iin=0;
volatile int16_t Iout=0;
volatile uint16_t old_Vin=0;
volatile uint16_t old_Iin=0;
volatile bool core0_ready = false;
volatile bool core1_ready = false;
/*
indica cosa sta facendo il core0 durante il suo funzionamento
0 - non avviato
1 - avviato
2 - sinistra di mpp
3 - destra di mpp
4 - su mpp
5 - dcdc mode, tensione di output troppo alta, mppt non possibile
10 - errore critico, riavviare
11 - errore, riavvio algoritmo
12 - backflow current in
13 - backflow current out
14 - backflow current in&out
*/
volatile uint8_t state=0;
//alla fine dei controlli l'algoritmo di MPPT guarda questo parametro
//se vero modifica il duty cycle aggiornandolo, se falso non fa nulla
volatile bool duty_change = false;

//core0 MPPT o DC-DC function
void setup(){
    state = 0;
    Serial.begin(250000);
    while (!Serial); //lasciare solo in fase di testing
    delay(100);
    
    Serial.print(F("\nStarting PWM_DynamicDutyCycle on ")); Serial.println(BOARD_NAME);
    Serial.println(RP2040_PWM_VERSION);
    
    PWM_Instance = new RP2040_PWM(digital_D6, frequency, dutyCycle);
    
    if (PWM_Instance){
    PWM_Instance->setPWM();
    }
    pinMode(digital_D10,OUTPUT);
    pinMode(pinLed,OUTPUT);
    analogReading();
    value_update();
    digitalWrite(digital_D10,HIGH);
    PWM_Instance->setPWM(digital_D6, frequency, dutyCycle+0.1f);
    state = 1;
}

//core1 Telemetry
void setup1(){
    u8g2.begin();
    pixels.begin();
    pinMode(neoPixel,OUTPUT);
    digitalWrite(neoPixel,HIGH);

    core1_ready = true;
    while(!core0_ready);
}
