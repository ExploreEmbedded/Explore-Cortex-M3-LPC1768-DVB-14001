#include<lpc17xx.h>

/* Configure the data bus and Control bus as per the hardware connection */
#define LcdDataBusPort      LPC_GPIO1->FIOPIN
#define LcdControlBusPort   LPC_GPIO2->FIOPIN

#define LcdDataBusDirnReg   LPC_GPIO1->FIODIR
#define LcdCtrlBusDirnReg   LPC_GPIO2->FIODIR


#define LCD_D0     20
#define LCD_D1     21
#define LCD_D2     22
#define LCD_D3     23
#define LCD_D4     24
#define LCD_D5     25
#define LCD_D6     26
#define LCD_D7     27

#define LCD_RS     0
#define LCD_RW     1
#define LCD_EN     2


/* Masks for configuring the DataBus and Control Bus direction */
#define LCD_ctrlBusMask  ((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN))
#define LCD_dataBusMask  ((1<<LCD_D0)|(1<<LCD_D1)|(1<<LCD_D2)|(1<<LCD_D3)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7))


/* local function to generate some delay */
void delay(int cnt)
{
    int i;
    for(i=0;i<cnt;i++);
}


/* Function send a complete byte on the Data bus */
void sendByte(char byte)
{
    LcdDataBusPort&=~(LCD_dataBusMask);                  // Clear previous data 
    LcdDataBusPort|= (((byte >>0x00) & 0x01) << LCD_D0);
    LcdDataBusPort|= (((byte >>0x01) & 0x01) << LCD_D1);
    LcdDataBusPort|= (((byte >>0x02) & 0x01) << LCD_D2);
    LcdDataBusPort|= (((byte >>0x03) & 0x01) << LCD_D3);
    LcdDataBusPort|= (((byte >>0x04) & 0x01) << LCD_D4);
    LcdDataBusPort|= (((byte >>0x05) & 0x01) << LCD_D5);
    LcdDataBusPort|= (((byte >>0x06) & 0x01) << LCD_D6);
    LcdDataBusPort|= (((byte >>0x07) & 0x01) << LCD_D7);
}


/* Function to send the command to LCD */
void Lcd_CmdWrite(char cmd)
{
    sendByte(cmd);                      //Send the Command
    LcdControlBusPort &= ~(1<<LCD_RS); // Send LOW pulse on RS pin for selecting Command register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1000);
    LcdControlBusPort &= ~(1<<LCD_EN);

    delay(10000);
}


/* Function to send the data to LCD */
void Lcd_DataWrite(char dat)
{
    sendByte(dat);                     //Send the data
    LcdControlBusPort |= (1<<LCD_RS);  // Send HIGH pulse on RS pin for selecting data register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1000);
    LcdControlBusPort &= ~(1<<LCD_EN);

    delay(10000);
}


int main()
{
    char i,a[]={"Good morning!"};

    SystemInit();                         //Clock and PLL configuration

    LcdDataBusDirnReg = LCD_dataBusMask;  // Configure all the LCD pins as output
    LcdCtrlBusDirnReg = LCD_ctrlBusMask;

    Lcd_CmdWrite(0x38);                   // enable 5x7 mode for chars 
    Lcd_CmdWrite(0x01);                   // Display OFF, Cursor ON
    Lcd_CmdWrite(0x0E);                   // Clear Display
    Lcd_CmdWrite(0x80);                   // Move the cursor to beginning of first line


    Lcd_DataWrite('H');
    Lcd_DataWrite('e');
    Lcd_DataWrite('l');
    Lcd_DataWrite('l');
    Lcd_DataWrite('o');
    Lcd_DataWrite(' ');
    Lcd_DataWrite('w');
    Lcd_DataWrite('o');
    Lcd_DataWrite('r');
    Lcd_DataWrite('l');
    Lcd_DataWrite('d');

    Lcd_CmdWrite(0xc0);

    for(i=0;a[i]!=0;i++)
    {
        Lcd_DataWrite(a[i]);
    }

    while(1);
}
