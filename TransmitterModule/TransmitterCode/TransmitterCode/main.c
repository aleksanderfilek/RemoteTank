/*
 * Transmitter.c
 *
 * Created: 09.08.2022 16:30:30
 * Author : filek
 */ 

#include "uart.h"
#include "avr/interrupt.h"
#include "audio.h"

const uint64_t pipe = 0xE8E8F0F0E1LL;
Nrf24Radio* radio;

void CallbackUart(uint8_t* data, uint16_t length)
{
	Nrf24Write(radio, data, length);
	
	uint8_t waitForResponse = data[1];
	if(waitForResponse == 0)
		return;
		
	Nrf24OpenReadingPipe(radio, pipe);
	Nrf24StartListening(radio);
	
	while(Nrf24Available(radio) == 0){}
		
	uint8_t buffer[32];
	Nrf24Read(radio, buffer, 32);
	UartArraySend(buffer, 32);
	
	Nrf24StopListening(radio);
	Nrf24OpenWritingPipe(radio, pipe);
}

int main(void)
{		
	//radio = (Nrf24Radio*)malloc(sizeof(Nrf24Radio));
	//Nrf24Init(radio);
	//
	UartInit(9600, 0);
	//UartCallbackSet(CallbackUart, COMMAND_LENGTH);
	//
	//SpiMasterInit();
	sei();
	//
	//if(Nrf24Begin(radio) == 0)
	//{
		//UartByteSend(ERR_RADIO_NOT_STARTED); // jakies zbieranie errrorowy by sie przydalo
		//return 0;
	//}
	//
	//Nrf24OpenWritingPipe(radio, pipe);
	AudioInit();
	
    while (1) 
    {
		if(UartCountRead() > 0)
		{
			char msg = UartRead();
			AudioBeep(2);
		}
    }
	
	//free(radio);
}

