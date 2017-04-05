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
	if (receivedDataCounter == 32){
		receivedDataCounter--;
	}
	receivedData[receivedDataCounter] = USART_ReceiveData(USART1); // принимаем данные
	receivedDataCounter++;
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(){
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);				// выключаем приём по USART
	if((receivedData[0] == 0x1d) & ((receivedData[2] == 0xa1) |(receivedData[2] == 0xa0)) & (receivedData[3] == 0x15)){	
		sendDataRS();
	}
	zeroReceivedData();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				// включаем приём по USART
}
