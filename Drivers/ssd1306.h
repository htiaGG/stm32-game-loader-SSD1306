#include "hal.h"

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_COLUMN_SIZE 128
#define SSD1306_PAGE_SIZE 4

// SSD1306 control byte
#define CONTROL_COMMAND 0x00
#define CONTROL_DATA    0x40

#define SSD1306_DISPLAY_OFF             0xAE
#define SSD1306_DISPLAY_ON              0xAF
#define SSD1306_SET_DISPLAY_CLOCK       0xD5
#define SSD1306_SET_MULTIPLEX           0xA8
#define SSD1306_SET_DISPLAY_OFFSET      0xD3
#define SSD1306_SET_START_LINE          0x40
#define SSD1306_CHARGE_PUMP             0x8D
#define SSD1306_MEMORY_MODE             0x20
#define SSD1306_SEG_REMAP               0xA0
#define SSD1306_COM_SCAN_DEC            0xC8
#define SSD1306_SET_COM_PINS            0xDA
#define SSD1306_SET_CONTRAST            0x81
#define SSD1306_SET_PRECHARGE           0xD9
#define SSD1306_SET_VCOM_DETECT         0xDB
#define SSD1306_DISPLAY_ALL_ON_RESUME   0xA4
#define SSD1306_NORMAL_DISPLAY          0xA6

#define COMMAND_COLUMN_ADDRESS          0x21
#define COMMAND_SET_PAGE_ADDRESS        0x22

void SSD1306_Send_Command(struct i2c *I2C, uint8_t address, uint8_t cmd)
{
    I2C_Begin(I2C, address, RW_Write);
    I2C_Write(I2C, CONTROL_COMMAND);
    I2C_Write(I2C, cmd);
    I2C_Stop(I2C);
}

// Initialize SSD1306 display with a sequence of commands
void SSD1306_Init(struct i2c *I2C) {
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_DISPLAY_OFF);             // Display off

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_DISPLAY_CLOCK);       // Set display clock
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x80);                            // Recommended oscillator frequency

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_MULTIPLEX);           // Set multiplex ratio
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x1F);                            // 1/64 duty

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_DISPLAY_OFFSET);      // Set display offset
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x00);                            // No offset

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_START_LINE | 0x00);   // Set start line at 0

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_CHARGE_PUMP);             // Enable charge pump
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x14);                            // Enable charge pump

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_MEMORY_MODE);             // Set memory addressing mode
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x00);                            // Horizontal addressing mode

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SEG_REMAP | 0x01);        // Set segment re-map
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_COM_SCAN_DEC);            // Set COM output scan direction

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_COM_PINS);            // Set COM pins hardware configuration
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x02);                            // Alternative COM pin configuration

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_CONTRAST);            // Set contrast control
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0xCF);                            // Max contrast

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_PRECHARGE);           // Set pre-charge period
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0xF1);                            // Phase 1 and 2 periods

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_SET_VCOM_DETECT);         // Set VCOMH deselect level
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, 0x40);                            // Default value

    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_DISPLAY_ALL_ON_RESUME);   // Resume to RAM content display
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_NORMAL_DISPLAY);          // Set normal display mode
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, SSD1306_DISPLAY_ON);              // Display on

    SSD1306_Clear_Display(I2C);
}


void SSD1306_Send_Buffer(struct i2c *I2C, const uint8_t* buffer, unsigned long length)
{
    I2C_Begin(I2C, SSD1306_I2C_ADDR, RW_Write);
    I2C_Write(I2C, CONTROL_DATA);
    for (uint32_t i = 0u; i < length; ++i)
    {
        I2C_Write(I2C, (buffer[i]));
    }
    I2C_Stop(I2C);
}

void SSD1306_Set_Column_Address(struct i2c *I2C, uint8_t start, uint8_t end)
{
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, COMMAND_COLUMN_ADDRESS);
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, start);
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, end);
}

void SSD1306_Set_Page_Address(struct i2c *I2C, uint8_t start, uint8_t end)
{
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, COMMAND_SET_PAGE_ADDRESS);
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, start);
    SSD1306_Send_Command(I2C, SSD1306_I2C_ADDR, end);
}

void SSD1306_Clear_Display(struct i2c *I2C)
{
    uint8_t page_buffer[128 * 8];
    memset(page_buffer, 0x00, sizeof(page_buffer));
    SSD1306_Set_Column_Address(I2C1, 0, SSD1306_COLUMN_SIZE - 1);
    SSD1306_Set_Page_Address(I2C1, 0, SSD1306_PAGE_SIZE - 1);
    SSD1306_Send_Buffer(I2C1, page_buffer, sizeof(page_buffer));
}
