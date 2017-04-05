#include "main.h"

int main(){
	initSendData();
	initAll();

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	while(1){
		//__NOP();
	}
}

void delay(uint32_t t) {
	uint32_t i = 0;
	for (; i < t; i++);
}

void USART1_IRQHandler(){
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);								 // очищаем флаг прерывания
	receivedData[receivedDataCounter] = USART_ReceiveData(USART1); // принимаем данные
	receivedDataCounter++;
	
	if ((receivedData[0] == 0x1d) & (receivedDataCounter == 1)){ 	 // если первый байт наш адрес
		//запускаем таймер 3 интервала
		return;
	} else {
		zeroReceivedData();
		return;
	}
	
	if((receivedData[1] == 0xa1) & (receivedDataCounter == 2)){
		//запускаем таймер 2 интервала
		return;
	} else {
		zeroReceivedData();
		//какие то действия
	}
	
	if((receivedData[2] == 0x15) & (receivedDataCounter == 3)){
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);				// выключаем приём
		sendDataRS();
		zeroReceivedData();
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				// включаем приём
		return;
	} else {
		zeroReceivedData();
		return;
	}
	
}
