//#include <lpcxx.h>

//#define NEW_HW


#include <lpc17xx.h>
#include <lpc17xx_systick.h>
#include <lpc17xx_clkpwr.h>
#include <lpc17xx_uart.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_clkpwr.h>
#include <core_cm3.h>

#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sys/printf.h"

#include <usbapi.h>


#include "sbl_iap.h"
#include "sbl_config.h"

#include "usb_desc.h"
#include "usbboot.h"


#define USB_CONNECT_PORT LPC_GPIO0
#define USB_CONNECT_PIN 11
//#define USB_CONNECT_INVERT

#define LED_PORT LPC_GPIO2
#define LED_PIN 8


#define SET_LED(led) (LED_PORT->FIOSET |= (1<<led))
#define CLR_LED(led) (LED_PORT->FIOCLR |= (1<<led))
#define TOGGLE_LED(led) (LED_PORT->FIOPIN ^= (1<<led))

#define DBG

#define CRP1  0x12345678
#define CRP2  0x87654321
#define CRP3  0x43218765
#define NOCRP 0x11223344

const uint32_t crp __attribute__((section(".crp"))) = NOCRP;

uint32_t time_ms;

void SysTick_Handler(void) {
	SYSTICK_ClearCounterFlag();

	LPC_WDT->WDFEED = 0xAA;
	LPC_WDT->WDFEED = 0x55;

	time_ms++;
}



void delay_busy(uint32_t time) {
	uint32_t new_time = time_ms + time;

	while(time_ms < new_time) CLKPWR_Sleep();
}


void vcom_putc(void* p, char c) {
	UART_Send(LPC_UART0, &c, 1, BLOCKING);
}


//usb interrupt handler
void USB_IRQHandler() {
	USBHwISR();
}



void uart_init() {

	UART_CFG_Type cfg;
	UART_FIFO_CFG_Type fifo_cfg;

	UART_ConfigStructInit(&cfg);
	cfg.Baud_rate = 57600;

	UART_Init(LPC_UART0, &cfg);

	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	UART_Init(LPC_UART0, &cfg);

	UART_FIFOConfigStructInit(&fifo_cfg);

	UART_FIFOConfig(LPC_UART0, &fifo_cfg);

	UART_TxCmd(LPC_UART0, ENABLE);

}

volatile uint8_t boot_flag;

void usbConnect(uint8_t con) {
	USBHwConnect(con);

#ifdef USB_CONNECT_PORT
#ifndef USB_CONNECT_INVERT
	if(con) {
		USB_CONNECT_PORT->FIOSET |= 1 << USB_CONNECT_PIN;
	} else {
		USB_CONNECT_PORT->FIOCLR |= 1 << USB_CONNECT_PIN;
	}
#else
	if(con) {
		USB_CONNECT_PORT->FIOCLR |= 1 << USB_CONNECT_PIN;
	} else {
		USB_CONNECT_PORT->FIOSET |= 1 << USB_CONNECT_PIN;
	}
#endif

#endif


}


int main() {
	//uart_init();
	//init_printf(NULL, vcom_putc);

	#ifdef LED_PORT
		LED_PORT->FIODIR |= 1<<LED_PIN;
		//LED_PORT->FIODIR |= 1<<9;
	#endif

	#ifdef LED_PORT
		LED_PORT->FIOCLR |= 1<<LED_PIN;
	#endif

	int rstsrc = LPC_SC->RSID & 0xF;
	uint8_t wdreset = (rstsrc & (1<<2)) || rstsrc == 0;

	LPC_SC->RSID = 0xF;

	if(!(LPC_GPIO2->FIOPIN & 1<<10)) {
		LPC_SC->RSID = 0xF;
		NVIC_SystemReset();
	}

	if(!wdreset && user_code_present() && !(LPC_GPIO2->FIODIR & (1<<10))) {
		//delay_busy(1000);
		execute_user_code();
	}

	SYSTICK_InternalInit(1);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);

#ifdef USB_CONNECT_PORT
	USB_CONNECT_PORT->FIODIR |= 1<<USB_CONNECT_PIN;
#ifdef USB_CONNECT_INVERT
	USB_CONNECT_PORT->FIOSET |= 1<<USB_CONNECT_PIN;
#endif
#endif



//	SYSTICK_InternalInit(1);
//	SYSTICK_IntCmd(ENABLE);
//	SYSTICK_Cmd(ENABLE);

	USBInit();

	USBRegisterDescriptors(abDescriptors);
	//USBHwRegisterDevIntHandler(on_usb_device_status);

	usb_boot_init();

	usbConnect(TRUE);

	//init_printf(NULL, vcom_putc);

	NVIC_EnableIRQ(USB_IRQn);

	//printf("labas\n");
	uint8_t timeout = 50;

	while(1) {

#ifdef LED_PORT
		delay_busy(80);
		LED_PORT->FIOSET |= 1<<LED_PIN;
		delay_busy(80);
		LED_PORT->FIOCLR |= 1<<LED_PIN;
#endif
		timeout--;
		if(timeout == 0 && wdreset) {
			//LPC_SC->RSID |= 1<<2; //clear wd reset bit
			usbConnect(FALSE);
			execute_user_code();
		}

		if(!(LPC_GPIO2->FIOPIN & 1<<10)) {
			NVIC_SystemReset();
		}

		if(boot_flag) {
			//printf("boot flag set\n");

			//printf("%X", *((uint32_t*)(USER_FLASH_START+4)));
			usbConnect(FALSE);

			execute_user_code();
		}

	}
}
