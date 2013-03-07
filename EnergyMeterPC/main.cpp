/*
 * main.cpp
 *
 *  Created on: Feb 28, 2013
 *      Author: walmis
 */
#include <lpc17xx_nvic.h>

#include <xpcc/architecture.hpp>
#include <xpcc/workflow.hpp>
#include <xpcc/debug.hpp>

#include <xpcc/driver/connectivity/wireless/at86rf230.hpp>
#include <xpcc/communication/TinyRadioProtocol.hpp>

using namespace xpcc;

const char fwversion[16] __attribute__((used, section(".fwversion"))) = "v0.11";

GPIO__OUTPUT(greenLed, 2, 9);
GPIO__OUTPUT(redLed, 2, 8);
GPIO__INPUT(progPin, 2, 10);
GPIO__OUTPUT(usbConnPin, 0, 11);

GPIO__OUTPUT(radioRst, 4, 28);
GPIO__OUTPUT(radioSel, 2, 0);
GPIO__IO(radioSlpTr, 0, 6);
GPIO__INPUT(radioIrq, 2, 1);

void boot_jump( uint32_t address ){
   __asm("LDR SP, [R0]\n"
   "LDR PC, [R0, #4]");
}

void sysTick() {
	LPC_WDT->WDFEED = 0xAA;
	LPC_WDT->WDFEED = 0x55;

	if(progPin::read() == 0) {
		//NVIC_SystemReset();

		NVIC_DeInit();

		LPC_WDT->WDFEED = 0x56;

		//boot_jump(0);
	}
}

xpcc::lpc17::USBSerial device(0xffff);

xpcc::IOStream stdout(device);
xpcc::log::Logger xpcc::log::debug(device);


xpcc::rf230::Driver<xpcc::lpc::SpiMaster1, radioRst, radioSel, radioSlpTr> rf_driver;

TinyRadioProtocol<typeof(rf_driver), AES_CCM_32> radio(rf_driver);

void Gpio2Handler(uint8_t pin, lpc17::IntEvent edge) {
	if(pin == 1) {
		rf_driver.IRQHandler();
	}
}



extern "C" void HardFault_Handler() {

	greenLed::set(0);

	//stdout.printf("fault\n");

	while(1) {
		//USB_IRQHandler();
	}
}

extern "C" void BusFault_Handler() {

	//greenLed::set(0);

	while(1);
}

int main() {
	greenLed::setOutput(1);
	redLed::setOutput(1);

	xpcc::Random::seed();

	lpc17::GpioInterrupt::enableInterrupt(2, 1, lpc17::IntSense::EDGE,
			lpc17::IntEdge::SINGLE, lpc17::IntEvent::RISING_EDGE);

	lpc17::GpioInterrupt::registerPortHandler(2, Gpio2Handler);

	lpc::SpiMaster1::configurePins();
	lpc::SpiMaster1::initialize(lpc::SpiMaster1::Mode::MODE_0,
			lpc::SpiMaster1::Prescaler::DIV002, 4);

	lpc17::SysTickTimer::enable();
	lpc17::SysTickTimer::attachInterrupt(sysTick);

	xpcc::PeriodicTimer<> t(500);

	usbConnPin::setOutput(true);
	device.connect();
	//hal.connect();


	//stdout.printf("clk %d\n", SystemCoreClock);

	radio.init();

	radio.setPanId(0x1234);
	radio.setAddress(0x3210);

	lpc17::GpioInterrupt::enableGlobalInterrupts();


	rf_driver.rxOn();

	int count = 0;
	while(1) {

		if(t.isExpired()) {
			//greenLed::toggle();
			redLed::toggle();

			//radio.send(0x0001, (uint8_t*)"labas", 5, 0, FrameType::DATA, TX_ACKREQ);


			//radio.sendFrame(f);

			//stdout.printf("labas %d\n", count++);

			//test.callVirtual();


		}
		radio.poll();


	}
}
