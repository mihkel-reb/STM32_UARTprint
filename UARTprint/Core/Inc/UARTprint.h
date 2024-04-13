//*********************************************
//UARTprint header file
//Assumes USART3 TX on PD8 is connected to ST-link
//Alter relevant bits according to your board/MCU
//***********************************************

#include "stm32f767xx.h"
#include <stdint.h>

#define USARTFreq 16000000
#define baudRate 115200


void initClocks(void) {
	//enable GPIOD clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	//enable USART3 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
}


void configPin(void) {
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

void configUART(void) {

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

	//USARTDIV = USARTCFreq / baudRate
	//USARTDIV = 16 000 000 / 115 200 = 139
	USART3->BRR = (int)(USARTFreq/baudRate);

	//enable UART3 TX
	USART3->CR1 |= (USART_CR1_TE);
	USART3->CR1 |= (USART_CR1_UE);

}

void initUART(void) {
	initClocks();
	configPin();
	configUART();
}

void UARTprint(const char *data) {
	initUART();

	while (*data) {

		while(!(USART3->ISR & USART_ISR_TXE));

		USART3->TDR = (*data & 0xFF);

		data++;
	}

}


