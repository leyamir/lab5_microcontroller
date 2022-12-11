/*
 * fsm.c
 *
 *  Created on: Dec 11, 2022
 *      Author: USER
 */

#include "fsm.h"
#include "main.h"
#include "software_timer.h"
#include <stdio.h>
uint8_t buffer[6];
uint8_t buffer_flag = 0;

const char send_command[6] = "!RST#";
const char ok_command[5] = "!OK#";

uint32_t ADC_value = 0;

int rst_pos = 0;
int ok_pos = 0;

char str[10];

void start_fsm() {
	switch (buffer_flag) {
		case 1:
			HAL_GPIO_TogglePin(GPIOA, LED_RED_Pin);
			if(send_command[rst_pos] == temp) {
				buffer[rst_pos++] = temp;
			}
			else rst_pos = 0;
			if(ok_command[ok_pos] == temp) {
				buffer[ok_pos] = temp;
				ok_pos++;
			}
			else ok_pos = 0;
			buffer_flag = 2;
			break;
		case 2:
			if(strcmp((const char*) buffer, "!RST#") == 0) {
				HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void*)str, sprintf(str , "\r\n!ADC=%d", (int) ADC_value), 1000);
				setTimer1(300);
				for(int i = 0; i < 6; i++) {
					buffer[i] = 0;
				}
				buffer_flag = 3;
			}
			break;
		case 3:
			if(strcmp((const char*) buffer, "!OK#") == 0) {
				setTimer1(0);
				buffer_flag = 0;
			}
			if(timer1_flag == 1) {
				setTimer1(300);
				HAL_UART_Transmit(&huart2, (void*)str, sprintf(str , "\r\n!ADC=%d", (int)ADC_value), 1000);
			}
			break;
	}
}

