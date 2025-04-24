#ifndef MCP3561_H
#define MCP3561_H

#include "stm32g4xx_hal.h"

#define MCP3561_CS_PORT		GPIOB
#define MCP3561_CS_PIN		GPIO_PIN_0

// MCP3561 Register Addresses

#define ADCDATA_REG 	0x0
#define CONFIG0_REG 	0x1
#define CONFIG1_REG 	0x2
#define CONFIG2_REG 	0x3
#define CONFIG3_REG 	0x4
#define IRQ_REG 		0x5
#define MUX_REG 		0x6
#define SCAN_REG 		0x7
#define TIMER_REG 		0x8
#define OFFSETCAL_REG 	0x9
#define GAINCAL_REG 	0xA
#define LOCK_REG 		0xD
#define CRCCFG_REG		0xF

#define MCP3561_SPI_ADDRESS 	0x01


// CONFIG0 Register Settings

#define VREF_SEL  		0x80		// Internal voltage reference is selected and buffered internally. REFIN+/OUT pin voltage is set at 2.4V (default)
#define CLK_SEL   		0x20		// Internal clock is selected and no clock output is present on the CLK pin
#define CS_SEL    		0x00		// No current source is applied to the ADC inputs (default)
#define ADC_MODE  		0x03 	// Conversion Mode

// CONFIG1 Register Settings

#define PRE  			0x00     // AMCLK = MCLK (default)
#define OSR 			0x3C 	// OSR: 98304

// CONFIG2 Register Settings

#define BOOST 			0x00	//00
#define GAIN			0x08	//default
#define AZ_MUX			0x04		 // 1 on
#define AZ_REF			0x02		// 1 - default

// CONFIG3 Register Settings

#define CONV_MODE		0xC0	//11
#define DATA_FORMAT 	0x00 // default format
#define CRC_FORMAT		0x00	// Disabled
#define EN_CRCCOM		0x00	// Disabled
#define EN_OFFCAL		0x00	// Disabled
#define EN_GAINCAL		0x00	// Disabled

//	IRQ Register

#define IRQ_MODE		0x00
#define EN_FASTCMD		0x02 // Default
#define EN_STP 			0x01 // Default

// MUX Register

//We change this in order to switch channels

#define MUX_CH0 		0x08 //Set MUX Register to read CH0
#define MUX_CH1 		0x18 //Set MUX Register to read CH1

// 	SCAN Register

#define DLY				 0x000000
#define SCAN_CHAN_SELECT 0x000003

// TIMER Register

#define TIMER 			 0x000000	//default

// OOFSETCAL Register

#define OFFSETCAL 		 0x000000

// GAINCAL Register

#define GAINCAL 		 0x800000

// MCP3561 Structure

typedef struct {

	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *csPort;
	int16_t csPin;

} MCP3561_Handle_t;

// Function Prototypes

void mcp3561_init(MCP3561_Handle_t *handle ,SPI_HandleTypeDef *hspi);

#endif


