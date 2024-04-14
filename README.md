# STM32_UARTprint
Simple bare-metal UART based print functionality for the NUCLEO-STM32F767ZI development board.
Allows for printing in serial consoles, similar to Arduino's Serial.println().
## Usage
* Add UARTprint.h to your project and include "UARTprint.h" in your main.c
* Initialize UARTprint using UARTprintInit()
* Use UARTprint() to print 
* View output on any serial console, such as Putty, on the STLink COM Port
## Demo
```c
#include "UARTprint.h"

int main(void) {

	UARTprintInit();

	UARTprint("Hello World!");

	while (1) {

		//...
	}
}
```
## Dependencies
* CMSIS for register definitions
