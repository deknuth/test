#include "main.h"


#define TMP116_W_ADDR	0x91
#define TMP116_R_ADDR	0x90


void PortInit(void)
{
    DDRB = 0B00000000;
    PORTB= 0B00000000;
    PINB = 0x00;

    DDRD = 0B01111100;		// PD5->RES	PD6->D/C PD7->BUSY PD4->SDI PD3->CS PD2->CLK
    PORTD= 0B00000000;
    PIND = 0x00;

    DDRC = 0B00101111;
    PORTC= 0B00110000;
    PINC = 0x00;
}

void TWI_WriteByte(unsigned char src)
{
    unsigned char i;
    SDA_116_OUT;
    for (i = 0; i < 8; i++)
    {
        if (src & 0x80)
            SDA_116_H;
        else
            SDA_116_L;
        SCL_116_H;
        _delay_us(2);
        SCL_116_L;
		_delay_us(1);
        src <<= 1;
    }
}

void TWI_WriteWord(unsigned int src)
{
    unsigned char i = 0;
    SDA_116_OUT;
    for (i = 0; i < 16; i++)
    {
        if (src & 0x8000)
            SDA_116_H;
        else
            SDA_116_L;
        SCL_116_H;
        _delay_us(2);
        SCL_116_L;
		_delay_us(1);
        src <<= 1;
    }
}

unsigned char TWI_ReadByte(void)
{
    unsigned char i, tmp=0;
    SDA_116_IN;
    for (i = 0; i < 8; i++)
    {
		if(SDA_116_HL)
            tmp = (tmp << 1) | 0x01;
        else
            tmp = tmp << 1;
		SCL_116_H;
        _delay_us(1);
        SCL_116_L;
		_delay_us(1);
    }
    return tmp;
}

unsigned int TWI_ReadWord(void)
{
    unsigned int regVal = 0;
    unsigned char i = 0;
    SDA_116_IN;
	_delay_us(1);
    for (i = 0; i < 16; i++)
    {
        if (SDA_116_HL)
            regVal = (regVal << 1) | 0x01;
        else
            regVal = regVal << 1;
		SCL_116_H;
        _delay_us(1);
        SCL_116_L;
		_delay_us(1);
    }
    return regVal;
}

void TMP116Init(void)
{
	TWI_WriteByte(TMP116_W_ADDR);	// IC slave address
	_delay_us(2);

	TWI_WriteByte(0x01);			// configure register address 0x01
	_delay_us(2);

	TWI_WriteWord(0x250);
	_delay_us(2);

}

unsigned int ReadTemp(void)
{
	unsigned int value = 0;
	unsigned char temp = 0;
	SDA_116_OUT;
	SCL_116_H;
	SDA_116_H;
	_delay_us(2);
	SDA_116_L;
	
	TWI_WriteByte(TMP116_W_ADDR);	// IC slave address

	SCL_116_H;
	_delay_us(2);
	SDA_116_L;


	TWI_WriteByte(0x0);				// data register address 0x00

	SDA_116_OUT;
	SCL_116_H;
	SDA_116_H;
	_delay_us(2);
	SDA_116_L;

	TWI_WriteByte(TMP116_R_ADDR);	// IC slave address

	SCL_116_H;
	_delay_us(2);
	SDA_116_L;

	
	temp = TWI_ReadByte();
	value = temp;
	value <<= 8;
	
	SCL_116_H;
	_delay_us(2);
	SDA_116_L;

	temp = TWI_ReadByte();
	value |= temp;
	return value;
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

int main(void)
{
 	unsigned int i;
	unsigned char str[2] = {0};
 	PortInit();
	UartInit();
	TWIInt();
	_delay_ms(300);
	SendStr("OK",2);
//	TMP116Init();
	while(1)
	{
	//	str[0] = I2C_Read(0x00);
	//	str[1] = I2C_Read(0x00);
		i = I2C_Read(0x00);
		str[0] = (i>>8);
		str[1] = i&0xFF;

		SendStr(str,2);
		_delay_ms(1400);
	}
}

