/*
 * MENU library
 * Created: 04.2020
 * Author : Mateusz Kozyra
 * Purpose : Part of project made at AGH UST.
 * Description : This library is for: visual data representation.
 *					All libraries required
 * Features : Presenting functionality of device at various screens
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "LCD.h"
#include "RTC.h"

/*******************************BUTTON PIN IN EXPANDER	******************/
#define MENU_BUTTON_OK		0X01
#define MENU_BUTTON_NEXT	0X02
#define MENU_BUTTON_BACK	0X04
/*************************************************************************/

/************************************************************************
 @brief Screen 1 contain time and date from RTC

 @param[in] state and substate - pointers to variable changed by interrupt
	it is used to change screen and interact with device
 @param[in] time and date - struct pointer - values from RTC
************************************************************************/
void menu_screen_time1(uint8_t *state, uint8_t *substate, struct Time *time, struct Date *date);

/************************************************************************
 @brief Screen 2 contain alarm information and allow to set alarm 

 @param[in] state and substate - pointers to variable changed by interrupt
	it is used to change screen and interact with device
 @param[in] time - struct pointer - value from RTC alarm
************************************************************************/
void menu_screen_alarm2(uint8_t *state, uint8_t *substate, struct Time *time);

/************************************************************************
 @brief Screen 3 contain sensor data

 @param[in] state and substate - pointers to variable changed by interrupt
	it is used to change screen and interact with device
************************************************************************/
void menu_screen_temperature3(uint8_t *state, uint8_t *substate);

/************************************************************************
 @brief Used to set default values between states

 @param[in] substate - pointers to variable changed by interrupt
	it is used to change screen and interact with device
************************************************************************/
void menu_interface_cleaning(uint8_t *substate);

/************************************************************************
 @brief Screen when alarm

 @param[in] state and substate - pointers to variable changed by interrupt
	it is used to change screen and interact with device
************************************************************************/
void menu_screen_alarm99(uint8_t *state, uint8_t *substate);
#endif /* MENU_H_ */
