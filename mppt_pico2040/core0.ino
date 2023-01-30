volatile float tmp;
void loop(){
    analogReading();
    if(Iin < 0 || Iout < 0){
        digitalWrite(digital_D10,LOW);
        if(Iin < 0 && Iout < 0){
               state = 14;
        }else{
            if(Iin < 0){
                state = 14;
            }else{
                state = 13;
            }
        }
    }else{
        if(VOUT_TARGET - Vout >= EPSELON_VOLTAGE){ //dcdc mode
            dutyCycle -= DUTY_VARIATION*2;
            duty_change = true;
            state  = 5;
        }else{//mppt mode
            if(Vin-old_Vin==0.0f){
                tmp = Iin - old_Iin;
                if(abs(tmp)>EPSELON_CURRENT){
                    if(tmp > 0){
                        dutyCycle -= DUTY_VARIATION;
                        state = 2;
                    }else{
                        dutyCycle += DUTY_VARIATION;
                        state = 3;
                    }
                    duty_change = true;
                }
            }else{
                tmp = ((Iin - old_Iin) / (Vin - old_Vin)) + (Iin / Vin);
                if(abs(tmp) > EPSELON){
                    if(tmp > 0){
                        dutyCycle -= DUTY_VARIATION;
                        state = 2;
                    }else{
                        dutyCycle += DUTY_VARIATION;
                        state = 3;
                    }
                    duty_change = true;
                }
            }
        }
        
        if(duty_change){
            PWM_Instance->setPWM(digital_D6, frequency, dutyCycle);
            duty_change = false;
        }else{
            state = 4;
        }
        value_update();
    }
    
    
}

/*
Legenda valori (da verificare con una taratura)
analog_A0
0 a 4096 = 0V a 38V per Vin

analog_A1
0 a 4096 = 0V a 62V per Vout

analog_A2 e analog_A3
-1240 a 2865 = -5A a +5A per Iin e Iout

*/
void analogReading(){
    Vin = analogRead(analog_A0);
    Vout = analogRead(analog_A1);
    Iin = analogRead(analog_A2)-1240;
    Iout = analogRead(analog_A3)-1240;
}

void value_update(){
    old_Vin = Vin;
    old_Iin = Iin;
}
