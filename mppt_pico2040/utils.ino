void printPWMInfo(RP2040_PWM* PWM_Instance){
    uint32_t div = PWM_Instance->get_DIV();
    uint32_t top = PWM_Instance->get_TOP();
    Serial.print("Actual PWM Frequency = "); Serial.println(PWM_Instance->getActualFreq());
    PWM_LOGDEBUG5("TOP =", top, ", DIV =", div, ", CPU_freq =", PWM_Instance->get_freq_CPU());
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
