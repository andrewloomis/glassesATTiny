#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "twi-no_stretch-tiny20-drvr.h"

// COMMENT THE LINE BELOW BEFORE COMPILING
#include <avr/iotn20.h>

#define BATT_CNTL PORTA5
#define PHONE_ON PORTA6

#define TOUCH_INPUT PINA4
#define FINGER_INPUT PINB2
#define SHUTDOWN_INPUT PINA3

#define TOUCH_INT PCINT4
#define FINGER_INT PCINT10

void turnOffPower()
{
    PORTA |= (1<<BATT_CNTL);
}

void turnOnPower()
{
    PORTA &= ~(1<<BATT_CNTL);
}

void turnOnSnapdragon()
{
    PORTA |= (1<<PHONE_ON);
    _delay_ms(50);
    PORTA &= ~(1<<PHONE_ON);
}

void activateSnapdragon()
{
    if (PINA & (1<<BATT_CNTL))
    {
        turnOnPower();
    }

    // If PHONE_ON_N signal is LOW
    else if (!(PINA & (1<<PHONE_ON)))
    {
        turnOnSnapdragon();
    }
}

ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT0_vect)
{
    // If touch slider interrupt is LOW
    if (!(PINA & (1<<TOUCH_INPUT)) || !(PINB & (1<<FINGER_INPUT)))
    {
        activateSnapdragon();
    }

    // If SHUTDOWN_INT is LOW
    if (!(PINA & (1<<SHUTDOWN_INPUT)))
    {
        _delay_ms(2000);
        turnOffPower();
    }
}

// ISR(PCINT1_vect)
// {
//     // If fingerprint sensor interrupt is LOW
//     if (!(PINB & (1<<FINGER_INPUT)))
//     {
//         activateSnapdragon();
//     }
// }

ISR(ANA_COMP_vect)
{
    turnOffPower();
}

int main()
{
    // Change clock freq to 8MHz
    CCP = 0xD8;
    CLKPSR = 0;

    // Set outputs (inputs are default)
    DDRA |= (1<<BATT_CNTL)|(1<<PHONE_ON);
    turnOffPower();

    // Shut down all peripherals except I2C
    PRR |= ~(1<<PRTWI);

    // Enable Pin Change Interrupts
    GIMSK |= (1<<PCIE0) | (1<<PCIE1);
    PCMSK1 |= (1<<FINGER_INT);
    PCMSK0 |= (1<<TOUCH_INT);

    // Set Analog Comparator Interrupt Mode to rising edge
    ACSRA |= (1<<ACIS0)|(1<<ACIS1);
    // Enable Analog Comparator Interrupt
    ACSRA |= (1<<ACIE);
    // 20mV hysteresis
    ACSRB |= (1<<HSEL);
    
    // Enable all interrupts
    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    while(1)
    {
        sleep_mode();
    }
}