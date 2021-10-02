#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t data = 0;
volatile uint8_t done = 0;
volatile uint8_t interrupt_count = 0;

#define CLOCK	(1<<PD5)
#define DIO		(1<<PD6)


void tick()
{
	PORTB ^= (1<<PB5);
	for (uint8_t i = 0; i < 55; i++)
	{
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
	
}

void init()
{
	DDRD = CLOCK | DIO;
	PORTD = CLOCK | DIO;
	DDRB = 1<<PB5;
}

void start()
{
	PORTD = CLOCK;
	tick();
	PORTD = 0;
	tick();
}

void send_bit(uint8_t bit)
{
	uint8_t val = bit ? DIO : 0;
	PORTD = val;
	tick();
	PORTD = val | CLOCK;
	tick();
	PORTD = val;
	tick();
}

void wait_for_ack()
{
	PORTD = 0;
	tick();
	DDRD = CLOCK;
	PORTD = CLOCK | DIO;
	while (PIND & DIO) {}
	DDRD = DIO | CLOCK;
}

void stop()
{
	PORTD = CLOCK;
	tick();
	PORTD = CLOCK | DIO;
	tick();
}

void send_byte(uint8_t data)
{
	for (uint8_t b = 1; b != 0; b <<= 1)
	{
		send_bit(data & b);
	}
	wait_for_ack();
}

void display_test()
{

	const uint8_t command1 = 0xAA;
	const uint8_t command3 = 0x8F;
	const uint8_t command2[] = {0xC0, 0x06, 0x5B, 0x4F, 0x66, 0x6D};
	init();
	for (uint8_t i = 0; i < 10; i++)
	{
		tick();
	}
	start();
	send_byte(command1);
	stop();
	tick();
	start();
	for (uint8_t i = 0; i < sizeof(command2); i++)
	{
		send_byte(command2[i]);
	}
	stop();
	tick();
	start();
	send_byte(command3);
	stop();
	
}

int main()
{
	while(1)
	{
		display_test();
		_delay_ms(1000);
	}
}

		/*const uint8_t buffer[] = {0x40, 0xC0, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x8F};
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
}*/