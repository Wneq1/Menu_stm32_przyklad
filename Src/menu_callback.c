#include <string.h>
#include "stm32l1xx_hal.h"

#include "Button.h"
#include "i2c-lcd-buf.h"
#include "menu.h"
#include "menu_callback.h"


#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

extern char lcd_buf[LCD_ROWS][LCD_COLS];

extern void (*key_next_func)(void);
extern void (*key_prev_func)(void);
extern void (*key_enter_func)(void);
extern void (*key_back_func)(void);

void disp_brightness_refresh(void);

void led_tog_callback(void){

	HAL_GPIO_TogglePin(LED_PORT, LED_PIN); // Toggle the LED state
	        HAL_Delay(500);
}

void disp_brightness_next(void){

	if(TIM4->CCR1 < 55000) TIM4->CCR1 += 5500;
	disp_brightness_refresh();

}

void disp_brightness_prev(void){

	if(TIM4->CCR1 >= 5500) TIM4->CCR1 -= 5500;
	disp_brightness_refresh();

}

void disp_brightness_back(void){

	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;

	menu_refresh();

}

void disp_brightness_refresh(void){

	uint8_t percent = (TIM4->CCR1)/550;

	buf_str_XY(0,0, "---- BRIGHTNESS ----");
	buf_clear_menu();

	memset(&lcd_buf[2][8],0xff,percent/10);
	memset(&lcd_buf[2][8+percent/10], '-',10-percent/10);

	buf_locate(2,2);
	buf_int(percent);
	buf_locate(5,2);
	buf_char('%');
}

void disp_brightness_callback(void){

	key_next_func = disp_brightness_next;
	key_prev_func = disp_brightness_prev;
	key_enter_func = NULL;
	key_back_func = disp_brightness_back;

	disp_brightness_refresh();

}
