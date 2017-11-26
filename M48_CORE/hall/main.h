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


#define SPI_SCS_H	PORTB|=1<<2
#define	SPI_SCS_L	PORTB&=~(1<<2)

#define	SPI_SCK_H	PORTB|=1<<1
#define	SPI_SCK_L	PORTB&=~(1<<1)

#define	SPI_DATA_OUT	DDRB|=1
#define	SPI_DATA_IN   	DDRB&=~1
#define	SPI_DATA_H	PORTB|=1
#define	SPI_DATA_L	PORTB&=~1
#define	SPI_DATA_HL	(PINB&0x01)
/*
#define	LED_ON		PORTB|=(1<<3)
#define	LED_OFF		PORTB&=~(1<<3)
#define	LED_BLINK	PORTB^=(1<<3)
*/
#define BAUD	115200

/************* TWI define ***********/
#define MAX 64
#if MAX > 255
	#define MAX 255
#endif

#define fSCL    150000    // TWIʱ��Ϊ50KHz Ԥ��Ƶϵ��=1(TWPS=0)
#if F_CPU < fSCL*36
	#define TWBR_SET    10     // TWBR������ڵ���10
#else
	#define TWBR_SET    (F_CPU/fSCL-16)/2 // ����TWBRֵ
#endif
#define TW_ACK		((1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA))
#define TW_NACK		((1<<TWINT) | (1<<TWEN) | (1<<TWIE))
#define TW_STO		((1<<TWINT) | (1<<TWEN) | (1<<TWSTO) | (1<<TWIE) | (1<<TWEA))
#define TW_STA		((1<<TWINT) | (1<<TWEN) | (1<<TWSTA) | (1<<TWIE) | (1<<TWEA))
#define TW_SAO		((1<<TWINT) | (1<<TWEN) | (1<<TWSTA) | (1<<TWSTO) | (1<<TWIE) | (1<<TWEA))
#define TR_ADD	0xB1		// high four bit = address, last bit read
#define TS_ADD	0xA0		// last bit write
/************************************/

#endif


