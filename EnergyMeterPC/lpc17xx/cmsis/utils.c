/*
 * utils.c
 *
 *  Created on: Mar 20, 2012
 *      Author: walmis
 */

#include "utils.h"
#include <inttypes.h>
#include <string.h>
#include <lpc17xx/cmsis/LPC17xx.h>


void _delay_us(uint32_t us) {
	int32_t ticks = (us*1000)/(1000000000UL/SystemCoreClock);
	if(ticks < 0) return;
	uint32_t cur = SysTick->VAL;

	while((cur - SysTick->VAL) < ticks);
}

void _delay_ms(uint32_t time) {
	_delay_us(1000);
}

typedef struct
{
  unsigned int ReturnCode;
  unsigned int Result[4];
}IAP_return_TypeDef;

//IAP
#define IAP_ADDRESS 0x1FFF1FF1

static void iap_entry(unsigned param_tab[], unsigned result_tab[]) {
	void (*iap)(unsigned[], unsigned[]);
	iap = (void(*)(unsigned[], unsigned[])) IAP_ADDRESS;
	iap(param_tab, result_tab);
}

int lpc17xx_read_serial_number(unsigned int ret[4]) //read serial via IAP
{
	unsigned param_table[5];
	IAP_return_TypeDef iap_return;
	param_table[0] = 58; //IAP command
	iap_entry(param_table, (unsigned int*) (&iap_return));

	if (iap_return.ReturnCode == 0)	{
		ret[0] = iap_return.Result[0];
		ret[1] = iap_return.Result[1];
		ret[2] = iap_return.Result[2];
		ret[3] = iap_return.Result[3];
		return 1;

	} else {
		return 0;
	}
}
