#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

GPIO_InitTypeDef gpio_b;        			//объявляем переменную для инициализации порта
USART_InitTypeDef usart;       				//объявляем переменную для инициализации USARTa

const uint32_t usartSpeed 	 = 115200;
const uint8_t bytesToSent 	 = 66;
const uint8_t bytesToReceive = 3;

uint8_t sendData[bytesToSent];  			//буфер содержащий информацию для передачи
uint8_t receivedData[bytesToReceive];	//буфер для чтения данных
uint8_t sendDataCounter = 0;    			//счётчик переданных байтов
uint8_t receivedDataCounter = 0;			//счётчик полученных байтов



void initAll(){
		__enable_irq();										//включаем прерывания
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		GPIO_StructInit(&gpio_b);					//настраиваем portB 6, 7 ножки
		
		gpio_b.GPIO_Mode = GPIO_Mode_AF;
		gpio_b.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		gpio_b.GPIO_Speed = GPIO_Speed_100MHz;
		gpio_b.GPIO_OType = GPIO_OType_PP;
		gpio_b.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &gpio_b);
	
		gpio_b.GPIO_Pin = GPIO_Pin_8;
    gpio_b.GPIO_Mode = GPIO_Mode_OUT;
    gpio_b.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_b.GPIO_OType = GPIO_OType_PP;
    gpio_b.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &gpio_b);
		
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
		
		
		USART_StructInit(&usart);					//настраиваем USART
		usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		usart.USART_BaudRate = usartSpeed;
		usart.USART_WordLength = USART_WordLength_8b;
		usart.USART_StopBits = USART_StopBits_1;
		usart.USART_Parity = USART_Parity_No;
		usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART1, &usart);
		
		NVIC_EnableIRQ(USART1_IRQn);
		USART_Cmd(USART1, ENABLE);
}

void initSendData(){
		sendData[0]  = 0x1d; 							//номер прибора
		sendData[1]  = 0xa1;							//флаг передачи данных
		/*for (int i=2; i<65; i++){
			sendData[2]  = 0x00;
		}*/
		sendData[2] = 0x15;//crc();							//crc

}

uint8_t crc(){
	uint8_t i = 0x00;
	return i;
}

void zeroReceivedData(){
	receivedData[0] 		= 0x00;
	receivedData[1] 		= 0x00;
	receivedData[2] 		= 0x00;
	receivedDataCounter = 0;
}

void sendDataRS(){
	for(sendDataCounter=0; sendDataCounter<bytesToSent; sendDataCounter++){
		USART_SendData(USART1, sendData[sendDataCounter]); 	// передача данных по USART
	}
}
