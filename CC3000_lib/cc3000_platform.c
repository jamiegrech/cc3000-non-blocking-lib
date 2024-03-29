/*
 * cc3000_platform.c
 *
 *  Created on: 05.09.2013
 *      Author: Johannes
 *
 *  Platform dependent functions
 *
 */

#include "board.h"
#include "cc3000_platform.h"
#include "TR868_SPI.h"

// guess what.. i am too fast.. so i need a delay
// if this is defined, then we add a little delay
// before CS_ASSERT and after CS_DEASSERT and before cc3000_read_irq_pin
//#define CS_IS_TOO_FAST

// CC3000 pin definition
const Pin pinCC3000_WL_EN = PIN_CC3000_WL_EN;
const Pin pinCC3000_EN = PIN_CC3000_EN;
const Pin pinCC3000_CS = PIN_CC3000_CS;
const Pin pinCC3000_IRQ = PIN_CC3000_IRQ;
static const Pin SpinsCC3000[] = {PINS_CC3000};

volatile uint8 cc3000_isr_flag = 0;
static void _cc3000_isr(const Pin* pPin);

// handle debug output
void debug_str(const char *string){
	printf("%s\r",string);
}
void debug_int(unsigned int wert){
	printf("%i",wert);
}
void debug_int_hex(unsigned char wert){
	printf("0x%02X ",wert);
}
void debug_putc(unsigned char wert){
	printf("%c",wert);
}
void debug_nl(void){
	printf("\n\r");
}
void debug_int_hex_16bit(uint16 wert){
	printf("0x%04X\n\r",wert);
}


//*****************************************************************************
//
//! delay_us
//!
//!  @param  usec    delay time in microseconds
//!
//!  @brief  delay in microseconds
//
//*****************************************************************************
#define BOARD_MCK_MHZ	48
void delay_us( uint32 usec){

	uint32 loop;
	loop = (BOARD_MCK_MHZ * usec)/6;
	while(loop--){
		__asm__("NOP");
	}
}
//*****************************************************************************
//
//! cc3000_isr
//!
//!  @brief  interrupt handler for CC3000 raises a flag if an interrupt occurs
//
//*****************************************************************************
static void _cc3000_isr(const Pin* pPin){

#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_isr\n");
#endif
	cc3000_isr_flag = 1;

}
//*****************************************************************************
//
//! cc3000_hw_setup
//!
//!  @brief  hardware setup for startup
//
//*****************************************************************************
void cc3000_hw_setup(void){

#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_hw_setup\n");
#endif
	//set Ports (EN = 0, WL_EN = 0, CS = 1)
	PIO_Configure( SpinsCC3000, PIO_LISTSIZE(SpinsCC3000));
	//init SPI
	halSpiInitSpi();
	//setup interrupt
	PIO_ConfigureIt( &pinCC3000_IRQ, _cc3000_isr);
	NVIC_EnableIRQ( (IRQn_Type)PIOA_IRQn );
	PIO_EnableIt( &pinCC3000_IRQ );

}
//*****************************************************************************
//
//! cc3000_set_pin_EN
//!
//!  @brief  sets the enable pin
//
//*****************************************************************************
void cc3000_set_pin_EN(void){

	PIO_Set( &pinCC3000_EN );
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_set_pin_EN\n");
#endif

}
//*****************************************************************************
//
//! cc3000_clear_pin_EN
//!
//!  @brief  sets the enable pin
//
//*****************************************************************************
void cc3000_clear_pin_EN(void){

	PIO_Clear( &pinCC3000_EN );
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_clear_pin_EN\n");
#endif

}
//*****************************************************************************
//
//! cc3000_set_pin_WL_EN
//!
//!  @brief  sets the enable pin
//
//*****************************************************************************
void cc3000_set_pin_WL_EN(void){

	PIO_Set( &pinCC3000_WL_EN );
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_set_pin_WL_EN\n");
#endif

}
//*****************************************************************************
//
//! cc3000_clear_pin_EN
//!
//!  @brief  sets the enable pin
//
//*****************************************************************************
void cc3000_clear_pin_WL_EN(void){

	PIO_Clear( &pinCC3000_WL_EN );
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_clear_pin_WL_EN\n");
#endif

}
//*****************************************************************************
//
//! cc3000_read_irq_pin
//!
//!  @brief    reads the state of the IRQ pin
//!
//!  @return   if pin is high 1 is returned else 0 is returned
//
//*****************************************************************************
uint8 cc3000_read_irq_pin(void) {
#ifdef CS_IS_TOO_FAST
	delay_us(100);
#endif
	return PIO_Get( &pinCC3000_IRQ ) ? 1 : 0;
}
//*****************************************************************************
//
//! cc3000_interrupt_enable
//!
//!  @brief  enables IRQ
//
//*****************************************************************************
void cc3000_interrupt_enable(void){

	PIO_EnableIt( &pinCC3000_IRQ ) ;
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_interrupt_ENable\n");
#endif

	if(!cc3000_read_irq_pin()){
		cc3000_isr_flag = 1;
	}


}
//*****************************************************************************
//
//! cc3000_interrupt_disable
//!
//!  @brief  disables IRQ
//
//*****************************************************************************
void cc3000_interrupt_disable(void){

	PIO_DisableIt( &pinCC3000_IRQ ) ;
#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_interrupt_DISable\n");
#endif

}
//*****************************************************************************
//
//! cc3000_assert_cs
//!
//!  @brief  asserts CS
//
//*****************************************************************************
void cc3000_assert_cs(void){

#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_Assert_cs\n");
#endif
	PIO_Clear( &pinCC3000_CS );
#ifdef CS_IS_TOO_FAST
	delay_us(50);
#endif
}
//*****************************************************************************
//
//! cc3000_deassert_cs
//!
//!  @brief  deasserts CS
//
//*****************************************************************************
void cc3000_deassert_cs(void){

#if (CC3000_TRACE_LEVEL & CC3000_TRACE_LEVEL_DEBUG)
	debug_str("cc3000_DEAssert_cs\n");
#endif
#ifdef CS_IS_TOO_FAST
	delay_us(50);
#endif
	PIO_Set( &pinCC3000_CS );

}

//*****************************************************************************
//
//! cc3000_spi_send
//!
//!  @param  data    data to send
//!
//!  @brief  sends 1 byte to the CC3000
//
//*****************************************************************************
void cc3000_spi_send(uint8 data){
	uint8 ret;
	uint32 txData;

	//wait until Transmit Data Register Empty
	while(!(SPI->SPI_SR & SPI_SR_TDRE));
	//prepare data for transfer
	txData = (uint32_t)data;
	//add CS to data
	txData |= SPI_PCS( 3 );
	//set tx register
	SPI->SPI_TDR = txData;
	//wait for answer
	while(!(SPI->SPI_SR & SPI_SR_RDRF));
	//read and return answer
	ret = (uint8)(SPI->SPI_RDR & 0xFFFF);
}

//*****************************************************************************
//
//! cc3000_spi_recv
//!
//!  @return  returns 1 byte
//!
//!  @brief  receives 1 byte from CC3000
//
//*****************************************************************************
uint8 cc3000_spi_recv(void){

	uint8 ret;
	uint32 txData = 0;
	//wait until Transmit Data Register Empty
	while(!(SPI->SPI_SR & SPI_SR_TDRE));
	//prepare data for transfer
	txData = (uint32_t)txData;
	//add CS to data
	txData |= SPI_PCS( 3 );
	//empty receiver register
	ret = (uint8)(SPI->SPI_RDR & 0xFFFF);
	//set tx register
	SPI->SPI_TDR = txData;
	//wait for answer
	while(!(SPI->SPI_SR & SPI_SR_RDRF));
	//read and return answer
	ret = (uint8)(SPI->SPI_RDR & 0xFFFF);
	return ret;
}

// EOF
