#ifndef MAIN_H_
#define MAIN_H_
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <math.h>
#include <util/delay.h>
#include <util/twi.h>

#define LOW           0
#define HIGH          (!LOW)
#define FALSE         0
#define TRUE		  (!FALSE)	

#define setbit(x,y) x |= (1<<y)
#define clrbit(x,y) x &= ~(1<<y)

#define	D1_ON		PORTC|=(1<<2)
#define	D1_OFF		PORTC&=~(1<<2)
#define	D1_BLINK	PORTC^=(1<<2)

#define	D2_ON		PORTC|=(1<<3)
#define	D2_OFF		PORTC&=~(1<<3)
#define	D2_BLINK	PORTC^=(1<<3)

#define SCL_116_H	PORTC|=(1<<5)
#define SCL_116_L	PORTC&=~(1<<5)

#define	SDA_116_OUT	DDRC|=(1<<4)
#define	SDA_116_IN  DDRC&=~(1<<4)
#define	SDA_116_H	PORTC|=(1<<4)
#define	SDA_116_L	PORTC&=~(1<<4)
#define	SDA_116_HL	(PINC&(1<<4))

#define BAUD	38400
extern void SendStr(unsigned char* data,unsigned char len);

extern void TWIInt(void);
extern unsigned char I2C_Write(unsigned char Wdata,unsigned char RegAddress);
extern unsigned int I2C_Read(unsigned RegAddress);

#endif

