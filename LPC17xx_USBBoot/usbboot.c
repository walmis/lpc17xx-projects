#include <lpc17xx.h>
#include <lpc17xx_systick.h>

#include <usbapi.h>
#include "usb_desc.h"
#include "usbboot.h"

#include <printf.h>
#include "sbl_iap.h"
#include "sbl_config.h"

static uint8_t buffer[64];

uint8_t flashing;
uint32_t size = 0;

static const unsigned key = 0x2eb5040b;

static uint32_t lfsr = 1;

static void srand(uint32_t seed) {
	if(!seed) seed = 1;
	lfsr = seed;
}

static uint32_t rand() {
	lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xD0000001u);
	return lfsr;
}

static uint8_t equals(char* a, char* b, int len) {
	int i;
	for(i = 0; i < len; i++) {
		if(a[i] != b[i]) return 0;
	}
	return 1;
}

uint32_t offset = 0;
//uint32_t

extern volatile uint8_t boot_flag;


static void BulkOut(U8 bEP, U8 bEPStatus) {

	uint8_t read = USBHwEPRead(bEP, buffer, sizeof(buffer));

	if(!flashing) {

		if(equals(":FLASH\n", buffer, 7)) {
			size = *(uint16_t*)&buffer[7];
			//printf("size %d\n", size);
			if(size & 511) {
				//printf("Number is not divisible by 512\n");
				USBHwEPWrite(BULK_IN_EP, "INVALID SIZE", 12);
				return;
			} else {
				flashing = TRUE;
				srand(key);
			}
		} else if(equals(":GETVERSION\n", buffer, 12)) {
			uint8_t *version = (uint8_t *)USER_FLASH_START + 0xCC;

			if(version[0] != 0xFF)
				USBHwEPWrite(BULK_IN_EP, version, 16);
			else {
				USBHwEPWrite(BULK_IN_EP, "UNK", 3);
			}
		} else if(equals(":BOOT\n", buffer, 6)) {
			boot_flag = TRUE;
		}

	} else {
		uint8_t *firmware = (uint8_t *)USER_FLASH_START + offset;
		int i;
		for(i = 0; i < read; i++) {
			buffer[i] ^= rand() & 0xFF;
		}

		uint8_t res = write_flash(firmware, buffer, read);
		//printf("write len-%d offset-%d result-%d\n", read, offset, res);
		offset += read;

		if(offset >= size) {
			int i;

			if(user_code_present()) {
				USBHwEPWrite(BULK_IN_EP, "WRITE OK VALID", 14);
			} else {
				USBHwEPWrite(BULK_IN_EP, "WRITE OK INVALID", 16);
			}

			offset = 0;
			flashing = 0;
		}
	}


}

static void BulkIn(U8 bEP, U8 bEPStatus)
{


}



void usb_boot_init() {

	USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);

}
/*
 * usbboot.c
 *
 *  Created on: Jan 14, 2013
 *      Author: walmis
 */


