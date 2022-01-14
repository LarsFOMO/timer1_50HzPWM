/*
        50Hz PWM-Signal
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>

void PWM1_INIT(void);
void Duty(uint16_t percentagex100, uint16_t ICR1_value, char pinSelect);
void FrequencyPWM(uint16_t frequency, uint16_t percentagex100, char pinSelect);

uint16_t divider = 256;

int main(void)
{
    uint16_t change = 0;
    PWM1_INIT();

    while(1)
    {
        FrequencyPWM(50, 1500, 'A');
        FrequencyPWM(50, 100, 'B');
    }

    return 0;
}

void PWM1_INIT(void)
{
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);

	TCCR1A=0xA2;
    TCCR1B=0x1B;
	TCNT1H=0x00;
	TCNT1L=0x00;
}

void Duty(uint16_t percentagex100, uint16_t ICR1_value, char pinSelect)
{
	percentagex100 =  (percentagex100 > 10000 ? 10000 : (percentagex100 < 0 ? 0 : percentagex100));       // ternary operator
	uint16_t OCR = (uint16_t)(((uint32_t)percentagex100 * (uint32_t)ICR1_value)/10000) ;    // Set pwm percent*100 of pwm period

    if(pinSelect == 'A')
    {
        OCR1AH = OCR >> 8;
        OCR1AL = OCR & 0xFF;
    }
    else if(pinSelect == 'B')
    {
        OCR1BH = OCR >> 8;
        OCR1BL = OCR & 0xFF;
    }

}

void FrequencyPWM(uint16_t frequency, uint16_t percentagex100, char pinSelect)
{
	uint16_t resolution = F_CPU/((uint32_t)divider * frequency);
	ICR1 = resolution - 1;
	if(pinSelect == 'A')
        OCR1A = (((uint32_t)percentagex100 * resolution) / 10000) - 1;
    else if(pinSelect == 'B')
        OCR1B = (((uint32_t)percentagex100 * resolution) / 10000) - 1;
}
