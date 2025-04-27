/*
 * MCP3561.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Arjun Kodial
 */

#include "Drivers/MCP3561.h"


void MCP3561_Init(MCP3561_Handle_t *handle ,SPI_HandleTypeDef *hspi){

	handle->hspi = hspi;
	handle->csPort = MCP3561_CS_PORT;
	handle->csPin = MCP3561_CS_PIN;

	//Full Reset of Device
	full_reset(handle);

	// 5ms delay after Reset
	HAL_Delay(5);

	//Set CONFIG0 Register
	write_reg(handle, CONFIG0_REG, VREF_SEL | CLK_SEL | CS_SEL | ADC_MODE);

	//Set CONFIG1 Register
	write_reg(handle, CONFIG1_REG, PRE | OSR);

	//Set CONFIG2 Register
	write_reg(handle, CONFIG2_REG, BOOST | GAIN | AZ_MUX | AZ_REF);

	//Set CONFIG3 Register
	write_reg(handle, CONFIG3_REG, CONV_MODE | DATA_FORMAT | CRC_FORMAT | EN_CRCCOM | EN_OFFCAL | EN_GAINCAL);

	//Set IRQ Register
	write_reg(handle, IRQ_REG, IRQ_MODE | EN_FASTCMD | EN_STP);

	//Set MUX Register, CH0 will be selected by default
	select_ch0(handle);


}


void MCP3561_Step(MCP3561_Handle_t *handle){




	uint8_t adc_data[4];

	read_buf(handle, ADCDATA_REG, adc_data, 4);

	// I use a signed integer as the ADC will output a signed value

	int32_t result =  (int32_t)adc_data[3] << 24 | (int32_t)adc_data[2] << 16 | (int32_t)adc_data[1] << 8 | adc_data[0];

	if(handle->current_channel == 0){
		handle->last_ch0_data = result;
	}
	else if(handle->current_channel == 1){
		handle->last_ch1_data = result;
	}

}


void select_ch1(MCP3561_Handle_t *handle){
	write_reg(handle, MUX_REG, MUX_CH1);
	handle->current_channel = 1;
}

void select_ch0(MCP3561_Handle_t *handle){
	write_reg(handle, MUX_REG, MUX_CH0);
	handle->current_channel = 0;
}

int32_t MCP3561_Get_CH0_Data(MCP3561_Handle_t *handle){
	return handle->last_ch0_data;
}
int32_t MCP3561_Get_CH1_Data(MCP3561_Handle_t *handle){
	return handle->last_ch1_data;
}

void read_buf(MCP3561_Handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len){

	//Start Communication by pulling CS PIN low
	HAL_GPIO_WritePin(handle->csPort,handle->csPin,GPIO_PIN_RESET);

	//Shift Reg address left two and set CMD[1:0] to 11 for Incremental Read
	reg = reg << 2;
	reg = reg | 0x3;

	//Transmit Read Command
	HAL_SPI_Transmit(handle->hspi,&reg,1,WRITE_TIMEOUT);

	// Read back data into the data buffer

	uint8_t temp_buf[len+1];

	HAL_SPI_Receive(handle->hspi, temp_buf, len+1, READ_TIMEOUT);

	//Write everything in temp_buf to buf except for status byte at index 0

	for(uint8_t i = 0; i < len; i++){
		buf[i] = temp_buf[i+1];
	}

	// End communication

	HAL_GPIO_WritePin(handle->csPort, handle->csPin, GPIO_PIN_SET);


}

uint8_t read_reg(MCP3561_Handle_t *handle, uint8_t reg){

	//Start Communication by pulling CS PIN low
	HAL_GPIO_WritePin(handle->csPort,handle->csPin,GPIO_PIN_RESET);

	//Shift Reg address left two and set LSB to 1 to prepare Read communication
	reg = reg << 2;
	reg = reg | 0x1;

	//Transmit Read Command
	HAL_SPI_Transmit(handle->hspi,&reg,1,WRITE_TIMEOUT);


	//Read back register data
	uint8_t register_data[2];

	HAL_SPI_Receive(handle->hspi, register_data, 2 ,READ_TIMEOUT);

	//End Communication by pulling CS Pin High
	HAL_GPIO_WritePin(handle->csPort, handle->csPin, GPIO_PIN_SET);


	//We only return register_data[1] because we skip the STATUS byte
	return register_data[1];

}

void write_reg(MCP3561_Handle_t *handle, uint8_t reg, uint8_t data){

	//Start Communication by pulling CS PIN low
	HAL_GPIO_WritePin(handle->csPort,handle->csPin,GPIO_PIN_RESET);

	//Shift Reg left two since address given in the CMD[5:2] bits

	reg = reg << 2;

	//Set CMD[1:0] = 10 for Incremental Write
	reg = reg & ~0x3; //Clear [1:0]
 	reg = reg | 0x2;  //Set [1:0]

 	//Transmit Write Command

 	uint8_t tx[2];

 	tx[0] = reg;
 	tx[1] = data;

 	HAL_SPI_Transmit(handle->hspi, tx, 2, WRITE_TIMEOUT);


 	//End communication
 	HAL_GPIO_WritePin(handle->csPort, handle->csPin, GPIO_PIN_SET);

}

void full_reset(MCP3561_Handle_t *handle){

	// See page 68 of MCP3561 for more info on Full Reset Command


	//Start Communication by pulling CS PIN low

	HAL_GPIO_WritePin(handle->csPort,handle->csPin,GPIO_PIN_RESET);

	//Transmit FULL_RESET_CMD

	uint8_t cmd = FULL_RESET_CMD;

	HAL_SPI_Transmit(handle->hspi, &cmd, 1 , WRITE_TIMEOUT);

	//End communication

	HAL_GPIO_WritePin(handle->csPort, handle->csPin, GPIO_PIN_SET);

}
