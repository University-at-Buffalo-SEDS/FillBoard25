#ifndef MCP3561_H
#define MCP3561_H

#include "stm32g4xx_hal.h"

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

#define MCP3561_I2C_ADDRESS 	0x01 //confirm address validity

// CONFIG0 Register Settings

#define VREF_SEL
#define CLK_SEL
#define CS_SEL
#define ADC_MODE 0x3

// CONFIG1 Register Settings

#define PRE
#define OSR

// CONFIG2 Register Settings

#define BOOST
#define GAIN
#define AZ_MUX
#define AZ_REF

// CONFIG3 Register Settings

#define CONV_MODE
#define DATA_FORMAT
#define CRC_FORMAT
#define EN_CRCCOM
#define EN_OFFCAL
#define EN_GAINCAL

// MCP3561 Structure

typedef struct {

	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *csPort;
	int16_t csPin;

} MCP3561_Handle_t;

// Function Prototypes

void mcp3561_init(MCP3561_Handle_t *handle ,SPI_HandleTypeDef *hspi);

#endif


