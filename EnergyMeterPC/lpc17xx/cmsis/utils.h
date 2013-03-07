/*
 * utils.h
 *
 *  Created on: Mar 20, 2012
 *      Author: walmis
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t time_ms;

void sleep_us(uint32_t us);
void sleep_ms(uint32_t time);

int lpc17xx_read_serial_number(unsigned int ret[4]);

#ifdef __cplusplus
}
#endif



#endif /* UTILS_H_ */
