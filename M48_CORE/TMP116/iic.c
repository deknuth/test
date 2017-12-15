#include "main.h"

/************* TWI define ***********/
#define MAX 64
#if MAX > 255
	#define MAX 255
#endif

#define fSCL    100000    // TWI时钟为50KHz 预分频系数=1(TWPS=0)
#if F_CPU < fSCL*36
	#define TWBR_SET    10     // TWBR必须大于等于10
#else
	#define TWBR_SET    (F_CPU/fSCL-16)/2 // 计算TWBR值
#endif

#define START    0x08
#define RE_START   0x10
#define MT_SLA_ACK   0x18
#define MT_SLA_NOACK 0x20
#define MT_DATA_ACK   0x28
#define MT_DATA_NOACK 0x30
#define MR_SLA_ACK   0x40
#define MR_SLA_NOACK 0x48
#define MR_DATA_ACK   0x50
#define MR_DATA_NOACK 0x58

#define RD_DEVICE_ADDR 0x91   //前4位器件固定,后三位看连线,最后1位是读写指令位
#define WD_DEVICE_ADDR 0x90

#define Start() 	(TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)) //启动I2C
#define Stop() 		(TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)) 	//停止I2C
#define Wait() 		{ while(!(TWCR&(1<<TWINT))); } 	//等待中断发生
#define TestAck() 	(TWSR&0xf8) 		// 观察返回状态
#define TW_ACK		((1<<TWINT) | (1<<TWEN) | (1<<TWEA))
#define TW_NACK		((1<<TWINT) | (1<<TWEN))
#define Twi() 		(TWCR=(1<<TWINT)|(1<<TWEN)) // 启动I2C
#define Write8Bit(x)	{TWDR=(x);TWCR=(1<<TWINT)|(1<<TWEN);} 		// 写数据到TWDR

void TWIInt(void)
{
    TWCR = 0x00;
    TWBR = TWBR_SET;	// set speed
    TWSR = 0x00;
}


/*********************************************
I2C总线写一个字节
返回0:写成功
返回1:写失败
**********************************************/
unsigned char I2C_Write(unsigned char Wdata,unsigned char RegAddress)
{
   Start();
   Wait();
   if(TestAck()!=START)
   return 1;     
  
   Write8Bit(WD_DEVICE_ADDR); //写I2C从器件地址和写方式
   Wait();
   if(TestAck()!=MT_SLA_ACK)
   return 1;     
  
   Write8Bit(RegAddress);   //写器件相应寄存器地址
   Wait();
   if(TestAck()!=MT_DATA_ACK)
   return 1;        
  
   Write8Bit(Wdata);    // 写数据到器件相应寄存器
   Wait();
   if(TestAck() != MT_DATA_ACK)
   return 1;      
  
   Stop();        //I2C停止
   _delay_ms(10);     //延时
   return 0;
} 

unsigned int I2C_Read(unsigned RegAddress)
{
	unsigned int temp;
	
	Start();
	Wait();
	if(TestAck()!=START) 
		return 1;
	
	Write8Bit(WD_DEVICE_ADDR); 	// 写I2C从器件地址和写方式
	Wait(); 
	if(TestAck()!=MT_SLA_ACK)
		return 1;
	
	Write8Bit(RegAddress); 	// 写器件相应寄存器地址
	Wait();
	if (TestAck()!=MT_DATA_ACK) 
		return 1;
	
	_delay_ms(150);
	
	Start(); 		// I2C重新启动
	Wait();
	if(TestAck()!=RE_START) 
		return 1;
	Write8Bit(RD_DEVICE_ADDR); // 写I2C从器件地址和读方式
	Wait();
	if(TestAck()!=MR_SLA_ACK) 
		return 1;

	Twi(); 			// 启动主I2C读方式
	Wait();
	if(TestAck() != MR_DATA_NOACK) 
		return 1; 
	temp = TWDR;		// 读取I2C接收数据
	temp <<= 8;
	
	Twi(); 			// 启动主I2C读方式
	Wait();
	temp |= TWDR;		// 读取I2C接收数据
	Stop();
	
	return temp;
}
