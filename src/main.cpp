#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t data = 0;
volatile uint8_t done = 0;
volatile uint8_t interrupt_count = 0;
int main()
{
	const uint8_t buffer[] = {0x40, 0xC0, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x8F};
	uint8_t index = 0;
	DDRB = 1 << PB5;
	DDRD = 0x60; // PB5-6 outputs
	PORTD = 0x60;
	data = buffer[index];
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TIMSK0 = 1 << OCIE0A;
	OCR0A = 15;
	TCNT0 = 0;
	PORTD = 1 << PD5;
	sei();
	TCCR0B = 1 << FOC0A | (1<<CS00);
	while(1)
	{
		if (done)
		{
			done = 0;
			DDRD = 1 << PD6;
			while((PIND & (1 << PD6)) != 0) {PORTD = 1 << PD5;;}
			PORTB = 1<<PB5;
			if (index == 0 || index == sizeof(buffer) - 1)
			{
				
				for (int x = 0; x < 32; x++)
				{
					asm ("nop");
				}
				DDRD = 0x60;
				data = buffer[index++];
				if (index >= sizeof(buffer))
				{
					
					asm ("sleep");
				}
				TCCR0B = 1 << FOC0A | (1<<CS00);
			}

		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	interrupt_count++;
	PORTB = 0;
	if (interrupt_count & 0x01)
	{
		if (data & 0x01)
		{
			PORTD |= 1 << PD6;
		}
		else
		{
			PORTD &= ~(1 << PD6);
		}
		data >>= 1;
	}

	if (interrupt_count >= 16)
	{
		interrupt_count = 0;
		done = 1;
		TCCR0B = 0;
	}
}