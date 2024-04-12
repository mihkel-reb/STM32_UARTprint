#include "stm32f767xx.h"
#include <stdint.h>
#include <string.h>

//USART3 TX on PD8

void delay(uint32_t delayCount) {
	uint32_t i = 0;
	for (i = 0; i < delayCount*10000; i++);
}

void initClocks(void) {
	//enable GPIOD clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	//enable USART3 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
}


void configurePin(void) {
	//clear bits for PD8
	GPIOD->MODER &= ~(1 << 16);
	GPIOD->MODER &= ~(1 << 17);

	//set PD8 to alternate function mode
	GPIOD->MODER |= (1 << 17);

	//set AF7 (0111) in AFRH for USART3TX function
	GPIOD->AFR[1] |= (1 << 0);
	GPIOD->AFR[1] |= (1 << 1);
	GPIOD->AFR[1] |= (1 << 2);
	GPIOD->AFR[1] &= ~(1 << 3);
}

void configureUART(void) {

	/*****Configure CR1 Register*****/

	//clear M1 for 1 start bit, 8 data bits
	USART3->CR1 &= ~(USART_CR1_M1);

	//inhibit receiver timeout & end of block interrupts
	USART3->CR1 &= ~(USART_CR1_RTOIE);
	USART3->CR1 &= ~(USART_CR1_EOBIE);

	//oversampling by 16
	USART3->CR1 &= ~(USART_CR1_OVER8);

	//inhibit character match interrupt
	USART3->CR1 &= ~(USART_CR1_CMIE);

	//disable mute mode
	USART3->CR1 &= ~(USART_CR1_MME);

	//clear M0 for 1 start bit, 8 data bits
	USART3->CR1 &= ~(USART_CR1_M0);

	//disable parity control
	USART3->CR1 &= ~(USART_CR1_PCE);

	//disable TX & RX for now
	USART3->CR1 &= ~(USART_CR1_TE);
	USART3->CR1 &= ~(USART_CR1_RE);

	//disable UART3 for now
	USART3->CR1 &= ~(USART_CR1_UE);

	/*****Configure CR2 Register*****/

	//disable receiver timeout
	USART3->CR2 &= ~(USART_CR2_RTOEN);

	//disable automatic baud rate detection
	USART3->CR2 &= ~(USART_CR2_ABREN);

	//transmit/receive LSB first
	USART3->CR2 &= ~(USART_CR2_MSBFIRST);

	//idle state high
	USART3->CR2 &= ~(USART_CR2_TXINV);
	USART3->CR2 &= ~(USART_CR2_RXINV);

	//disable pin swap
	USART3->CR2 &= ~(USART_CR2_SWAP);

	//disable lIN mode
	USART3->CR2 &= ~(USART_CR2_LINEN);

	//1 stop bit
	USART3->CR2 &= ~(1 << 12);
	USART3->CR2 &= ~(1 << 13);

	//async mode
	USART3->CR2 &= ~(USART_CR2_CLKEN);

	//disable LIN interrupt
	USART3->CR2 &= ~(USART_CR2_LBDIE);

	/*****Configure CR3 Register*****/

	//inhibit transmission complete before guard time interrupt
	USART3->CR3 &= ~(1 << 24);

	//disable driver enable mode
	USART3->CR3 &= ~(USART_CR3_DEM);

	//disable various other modes
	USART3->CR3 &= ~(0x7F << 3);

	//disable IrDA mode
	USART3->CR3 &= ~(USART_CR3_IREN);

	//inhibit error interrupt
	USART3->CR3 &= ~(USART_CR3_EIE);

	/*****Set baud rate & enable UART*****/

	//USARTDIV = USARTClockFreq / baud
	//USARTDIV = 16 000 000 / 115 200 = 139
	USART3->BRR = 139;

	//enable UART3 TX
	USART3->CR1 |= (USART_CR1_TE);
	USART3->CR1 |= (USART_CR1_UE);

}

void UARTtransmit(const char *data) {

	while (*data) {

		while(!(USART3->ISR & USART_ISR_TXE));

		USART3->TDR = (*data & 0xFF);

		data++;
	}

}

void initUART(void) {
	initClocks();
	configurePin();
	configureUART();
}

int main(void) {

	initUART();

	while (1) {

		UARTtransmit("Hello World");
		delay(100);

	}

}
