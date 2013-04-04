/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2010, Roel Verdult
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// The GCC compiler defines the current architecture derived from the -mcpu argument.
// When target cpu is the cortex-m3, it automatically defines __ARM_ARCH_7M__
#ifndef __ARM_ARCH_7M__
  #error "The target ARM cpu must be Cortex-M3 compatible (-mcpu=cortex-m3)"
#endif

// Declare a weak alias macro as described in the GCC manual[1][2]
#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
#define SECTION(s) __attribute__ ((section(s)))

/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/

void fault_undefined() {
  // Do nothing when occured interrupt is not defined, just keep looping
  while(1);
}



/*****************************************************************************
 * Forward undefined fault handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 ****************************************************************************/



extern void default_irq_handler();

void irq_undefined() {
  // Do nothing when occured interrupt is not defined, just keep looping
	default_irq_handler();
}

void NMI_Handler(void)          WEAK_ALIAS(fault_undefined);
void HardFault_Handler(void)    WEAK_ALIAS(fault_undefined);
void MemManage_Handler(void)    WEAK_ALIAS(fault_undefined);
void BusFault_Handler(void)     WEAK_ALIAS(fault_undefined);
void UsageFault_Handler(void)   WEAK_ALIAS(fault_undefined);
void SVCall_Handler(void)       WEAK_ALIAS(fault_undefined);
void DebugMon_Handler(void)     WEAK_ALIAS(fault_undefined);
void PendSV_Handler(void)       WEAK_ALIAS(fault_undefined);
void SysTick_Handler(void)      WEAK_ALIAS(fault_undefined);

void WDT_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 16: Watchdog Timer               */
void TIMER0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);         /* 17: Timer0                       */
void TIMER1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);         /* 18: Timer1                       */
void TIMER2_IRQHandler(void)   WEAK_ALIAS(irq_undefined);         /* 19: Timer2                       */
void TIMER3_IRQHandler(void)   WEAK_ALIAS(irq_undefined);         /* 20: Timer3                       */
void UART0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 21: UART0                        */
void UART1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 22: UART1                        */
void UART2_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 23: UART2                        */
void UART3_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 24: UART3                        */
void PWM1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 25: PWM1                         */
void I2C0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 26: I2C0                         */
void I2C1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 27: I2C1                         */
void I2C2_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 28: I2C2                         */
void SPI_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 29: SPI                          */
void SSP0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 30: SSP0                         */
void SSP1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 31: SSP1                         */
void PLL0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 32: PLL0 Lock (Main PLL)         */
void RTC_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 33: Real Time Clock              */
void EINT0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 34: External Interrupt 0         */
void EINT1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 35: External Interrupt 1         */
void EINT2_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 36: External Interrupt 2         */
void EINT3_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 37: External Interrupt 3         */
void ADC_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 38: A/D Converter                */
void BOD_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 39: Brown-Out Detect             */
void USB_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 40: USB                          */
void CAN_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 41: CAN                          */
void DMA_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 42: General Purpose DMA          */
void I2S_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 43: I2S                          */
void ENET_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 44: Ethernet                     */
void RIT_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 45: Repetitive Interrupt Timer   */
void MCPWM_IRQHandler(void)   WEAK_ALIAS(irq_undefined);          /* 46: Motor Control PWM            */
void QEI_IRQHandler(void)   WEAK_ALIAS(irq_undefined);            /* 47: Quadrature Encoder Interface */
void PLL1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);           /* 48: PLL1 Lock (USB PLL)          */
void USBActivity_IRQHandler(void)   WEAK_ALIAS(irq_undefined);		/* 49: USB Activity 				*/
void CANActivity_IRQHandler(void)   WEAK_ALIAS(irq_undefined);		/* 50: CAN Activity					*/


/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/

// Prototype the entry values, which are handled by the linker script
extern void __stack_end;

extern void boot_entry(void);

// Defined irq vectors using simple c code following the description in a white 
// paper from ARM[3] and code example from Simonsson Fun Technologies[4].
// These vectors are placed at the memory location defined in the linker script
const void *vectors[] SECTION(".irq_vectors") =
{
  // Stack and program reset entry point
  &__stack_end,          // The initial stack pointer
  boot_entry,            // The reset handler

  // Various fault handlers
  NMI_Handler,           // The NMI handler
  HardFault_Handler,     // The hard fault handler
  MemManage_Handler,     // The MPU fault handler
  BusFault_Handler,      // The bus fault handler
  UsageFault_Handler,    // The usage fault handler
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  SVCall_Handler,        // SVCall handler
  DebugMon_Handler,      // Debug monitor handler
  0,                     // Reserved
  PendSV_Handler,        // The PendSV handler
  SysTick_Handler,       // The SysTick handler

  WDT_IRQHandler,             /* 16: Watchdog Timer               */
  TIMER0_IRQHandler,          /* 17: Timer0                       */
  TIMER1_IRQHandler,          /* 18: Timer1                       */
  TIMER2_IRQHandler,          /* 19: Timer2                       */
  TIMER3_IRQHandler,          /* 20: Timer3                       */
  UART0_IRQHandler,           /* 21: UART0                        */
  UART1_IRQHandler,           /* 22: UART1                        */
  UART2_IRQHandler,           /* 23: UART2                        */
  UART3_IRQHandler,           /* 24: UART3                        */
  PWM1_IRQHandler,            /* 25: PWM1                         */
  I2C0_IRQHandler,            /* 26: I2C0                         */
  I2C1_IRQHandler,            /* 27: I2C1                         */
  I2C2_IRQHandler,            /* 28: I2C2                         */
  SPI_IRQHandler,             /* 29: SPI                          */
  SSP0_IRQHandler,            /* 30: SSP0                         */
  SSP1_IRQHandler,            /* 31: SSP1                         */
  PLL0_IRQHandler,            /* 32: PLL0 Lock (Main PLL)         */
  RTC_IRQHandler,             /* 33: Real Time Clock              */
  EINT0_IRQHandler,           /* 34: External Interrupt 0         */
  EINT1_IRQHandler,           /* 35: External Interrupt 1         */
  EINT2_IRQHandler,           /* 36: External Interrupt 2         */
  EINT3_IRQHandler,           /* 37: External Interrupt 3         */
  ADC_IRQHandler,             /* 38: A/D Converter                */
  BOD_IRQHandler,             /* 39: Brown-Out Detect             */
  USB_IRQHandler,             /* 40: USB                          */
  CAN_IRQHandler,             /* 41: CAN                          */
  DMA_IRQHandler,             /* 42: General Purpose DMA          */
  I2S_IRQHandler,             /* 43: I2S                          */
  ENET_IRQHandler,            /* 44: Ethernet                     */
  RIT_IRQHandler,             /* 45: Repetitive Interrupt Timer   */
  MCPWM_IRQHandler,           /* 46: Motor Control PWM            */
  QEI_IRQHandler,             /* 47: Quadrature Encoder Interface */
  PLL1_IRQHandler,            /* 48: PLL1 Lock (USB PLL)          */
  USBActivity_IRQHandler,		/* 49: USB Activity 				*/
  CANActivity_IRQHandler,		/* 50: CAN Activity					*/
};

/******************************************************************************
 * References
 *  [1] http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *  [2] http://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
 *  [3] http://www.arm.com/files/pdf/Cortex-M3_programming_for_ARM7_developers.pdf
 *  [4] http://fun-tech.se/stm32/OlimexBlinky/mini.php
 *****************************************************************************/

