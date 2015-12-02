#include "uart.h"


void main()
{

    SystemInit();

    UART_Init(0,9600);   // Initilaize All the UARTs at 9600 Baud rate
    UART_Init(1,9600);   // Both UART_Init(0,9600) and UART0_Init(9600) are same
    UART2_Init(9600);	 // Internally UART0_Init() is mapped to UART_Init()
    UART3_Init(9600);	 // You can notice First two(0,1) are called with parameter other two(2,3) with suffix

    /*Directly use the Channel suffixed interface UART0_Printf, where suffix "0" specifies the UART channel*/
    UART0_Printf("Channel Zero %4d,  %2x\n\r", 1234, 0xabcd); 
    UART1_Printf("Channel One %4d,   %2x\n\r", 5678, 0xdd);
    UART2_Printf("Channel Two %4d,   %2x\n\r", 3421, 0xee);
    UART3_Printf("Channel Three %4d, %2x\n\r", 8912, 0xff);


    /*Use the standard interface by passing the channel number as first paramater*/
    UART_Printf(0,"Channel Zero %4d,  %2x\n\r", 1234, 0xabcd);
    UART_Printf(1,"Channel One %4d,   %2x\n\r", 5678, 0xdd);
    UART_Printf(2,"Channel Two %4d,   %2x\n\r",  3421, 0xee);
    UART_Printf(3,"Channel Three %4d, %2x\n\r",8912, 0xff);

    while(1);
}
