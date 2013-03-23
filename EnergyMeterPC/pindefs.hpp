/*
 * pindefs.hpp
 *
 *  Created on: Mar 15, 2013
 *      Author: walmis
 */

#ifndef PINDEFS_HPP_
#define PINDEFS_HPP_

#include <xpcc/architecture.hpp>

GPIO__OUTPUT(_greenLed, 2, 9);
GPIO__OUTPUT(_redLed, 2, 8);

typedef xpcc::gpio::Invert< _greenLed > greenLed;
typedef xpcc::gpio::Invert< _redLed > redLed;

GPIO__INPUT(progPin, 2, 10);
GPIO__OUTPUT(usbConnPin, 0, 11);

GPIO__OUTPUT(radioRst, 4, 28);
GPIO__OUTPUT(radioSel, 2, 0);
GPIO__IO(radioSlpTr, 0, 6);
GPIO__INPUT(radioIrq, 2, 1);

#endif /* PINDEFS_HPP_ */
