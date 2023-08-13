
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup 0x27/0x4E

void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  // en=1, rs=0
    data_t[1] = data_u | 0x08;  // en=0, rs=0
    data_t[2] = data_l | 0x0C;  // en=1, rs=0
    data_t[3] = data_l | 0x08;  // en=0, rs=0
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)data_t, 4, 100);
}

void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  // en=1, rs=1
    data_t[1] = data_u | 0x09;  // en=0, rs=1
    data_t[2] = data_l | 0x0D;  // en=1, rs=1
    data_t[3] = data_l | 0x09;  // en=0, rs=1
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)data_t, 4, 100);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x80);
    for (int i = 0; i < 80; i++)
    {
        lcd_send_data(' ');
    }
}

void lcd_put_cur(int row, int col)
{
    static const int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_init(void)
{
    // Inicjalizacja trybu 4-bitowego
    HAL_Delay(50);  // czekaj >40 ms
    lcd_send_cmd(0x30);
    HAL_Delay(5);  // czekaj >4.1 ms
    lcd_send_cmd(0x30);
    HAL_Delay(1);  // czekaj >100 µs
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20);  // tryb 4-bitowy
    HAL_Delay(10);

    // Inicjalizacja wyświetlacza
    lcd_send_cmd(0x28); // Funkcja ustawienia --> DL=0 (tryb 4-bitowy), N = 1 (2-linijny wyświetlacz), F = 0 (znaki 5x8)
    HAL_Delay(1);
    lcd_send_cmd(0x08); // Kontrola włączania/wyłączania wyświetlacza --> D=0, C=0, B=0 (wyłącz wyświetlacz)
    HAL_Delay(1);
    lcd_send_cmd(0x01); // Wyczyść wyświetlacz
    HAL_Delay(1);
    HAL_Delay(1);
    lcd_send_cmd(0x06); // Ustawienia trybu wprowadzania --> I/D = 1 (inkrementuj kursor) i S = 0 (brak przesunięcia)
    HAL_Delay(1);
    lcd_send_cmd(0x0C); // Kontrola włączania/wyłączania wyświetlacza --> D = 1, C i B = 0 (Kursor i migotanie, ostatnie dwa bity)
}

void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}
