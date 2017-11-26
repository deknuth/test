#include "main.h"


#define D1_BLINK	PORTC^=0x04
#define D1_ON		PORTC|=0x04
#define D1_OFF		PORTC&=~0x04


void PortInit(void)
{
    DDRB = 0B00000000;
    PORTB= 0B00000000;
    PINB = 0x00;

    DDRD = 0B00000000;
    PORTD= 0B00000000;
    PIND = 0x00;

    DDRC = 0B00111100;
    PORTC= 0B00000000;
    PINC = 0x00;
}

void UartInit(void)
{
    UBRR0H = (F_CPU / BAUD / 16 - 1) / 256;
    UBRR0L = (F_CPU / BAUD / 16 - 1) % 256;
    UCSR0B = 1<<RXEN0 | 1<<TXEN0 | 1<<RXCIE0;
    UCSR0C = 1<<UCSZ00 | 1<<UCSZ01;
}

void SendStr(unsigned char* data,unsigned char len)
{
    unsigned char i;
    for(i=0; i<len; i++)
    {
        while(!(UCSR0A & (1 << UDRE0)));
        UDR0 = *(data++);
    }
}


void I0Init(void)
{
    EICRA = 1<<ISC01;	
	EIMSK |= 1<<INT0;
}


ISR(INT0_vect)
{
	D1_BLINK;
}

/*
ISR(USART_RX_vect)
{
	word =UDR0;
	if(word == 0xFE)
	{
		if(!U0Ready)
			start = 1;
	}
	if(start)
	{
		U0Buf[U0Count++] = word;
		if(U0Count == zLen)
		{
			U0Ready = 1;
			start = zLen = 0;
		}
		else if(U0Count == 2)
		{
			if(word < 19)		// 协议最大长度
				zLen = word;
			else
			{
				U0Count = 0;
				start = 0;
			}
		}
	}
}
*/

int main(void)
{
	
	cli();
    PortInit();
    I0Init();
	sei();
	D1_BLINK;
	_delay_ms(500);
	D1_BLINK;
	_delay_ms(500);
	D1_BLINK;
	_delay_ms(500);
	D1_BLINK;
	_delay_ms(500);
	D1_OFF;
	while(1)
    {
		;
	}

}