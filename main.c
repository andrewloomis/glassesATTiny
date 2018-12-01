#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

// #include <avr/iotn20.h>

#define BATT_CNTL PORTA5
#define PHONE_ON PORTA6
#define TOUCH_INT PINA4
#define FINGER_INT PINB2

int main()
{
    // Set outputs (inputs are default)
    DDRA |= (1<<BATT_CNTL)|(1<<PHONE_ON);

    // If fingerprint sensor or touch slider interrupt is HIGH
    if ((PINA & (1<<TOUCH_INT)) || (PINB & (1<<FINGER_INT)))
    {
        // If PHONE_ON_N signal is LOW
        if (!(PINA & (1<<PHONE_ON)))
        {
            PORTA |= (1<<BATT_CNTL);
        }
        PORTA |= (1<<BATT_CNTL);
    }
}