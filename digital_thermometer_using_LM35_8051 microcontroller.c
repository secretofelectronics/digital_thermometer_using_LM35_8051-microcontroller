#include <reg51.h>

sbit rs = P2 ^ 7;   // Register Select(RS) pin of 16x2 LCD
sbit rw = P2 ^ 6;   // Read/Write(RW) pin of 16x2 LCD
sbit en = P2 ^ 5;   // Enable(E) pin of 16x2 LCD

sbit rd_adc = P3 ^ 0;   // Read(RD) pin of ADC0804
sbit wr_adc = P3 ^ 1;   // Write(WR) pin of ADC0804
sbit intr_adc = P3 ^ 2; // Interrupt(INTR) pin of ADC0804

void delay(unsigned int);  // Function for creating delay
void cmdwrt(unsigned char);  // Function for sending commands to 16x2 LCD display
void datawrt(unsigned char); // Function for sending data to 16x2 LCD display
void convert_display(unsigned char); // Function for converting ADC value to temperature and displaying it on 16x2 LCD display

void main(void) {
    unsigned char i;
    unsigned char cmd[] = {0x38, 0x01, 0x06, 0x0c, 0x82}; // 16x2 LCD initialization commands
    unsigned char data1[] = "Temperature:";
    unsigned char value;

    P1 = 0xFF; // Make Port 1 as input port
    P0 = 0x00; // Make Port 0 as output port

    for (i = 0; i < 5; i++) { // Send commands to 16x2 LCD display one command at a time
        cmdwrt(cmd[i]); // Function call to send commands to 16x2 LCD display
        delay(1);
    }

    for (i = 0; i < 12; i++) { // Send data to 16x2 LCD display one character at a time
        datawrt(data1[i]); // Function call to send data to 16x2 LCD display
        delay(1);
    }

    intr_adc = 1; // Make INTR pin as input
    rd_adc = 1;   // Set RD pin HIGH
    wr_adc = 1;   // Set WR pin LOW

    while (1) { // Repeat forever
        wr_adc = 0; // Send LOW to HIGH pulse on WR pin
        delay(1);
        wr_adc = 1;
        while (intr_adc == 1); // Wait for End of Conversion
        rd_adc = 0; // Make RD = 0 to read the data from ADC0804
        value = P1; // Copy ADC data
        convert_display(value); // Function call to convert ADC data into temperature and display it on 16x2 LCD display
        delay(1000); // Interval between every cycle
        rd_adc = 1;  // Make RD = 1 for the next cycle
    }
}

void cmdwrt(unsigned char x) {
    P0 = x;  // Send the command to Port 0 on which 16x2 LCD is connected
    rs = 0;  // Make RS = 0 for command
    rw = 0;  // Make RW = 0 for write operation
    en = 1;  // Send a HIGH to LOW pulse on Enable(E) pin to start command write operation 
    delay(1);
    en = 0;
}

void datawrt(unsigned char y) {
    P0 = y; // Send the data to Port 0 on which 16x2 LCD is connected
    rs = 1; // Make RS = 1 for command
    rw = 0; // Make RW = 0 for write operation
    en = 1; // Send a HIGH to LOW pulse on Enable(E) pin to start data write operation
    delay(1);
    en = 0;
}

void convert_display(unsigned char value) {
    unsigned char x1, x2, x3;

    cmdwrt(0xc6);  // Command to set the cursor to 6th position of 2nd line on 16x2 LCD

    x1 = (value / 10); // Divide the value by 10 and store quotient in variable x1
    x1 = x1 + (0x30); // Convert variable x1 to ASCII by adding 0x30
    x2 = value % 10; // Divide the value by 10 and store remainder in variable x2
    x2 = x2 + (0x30); // Convert variable x2 to ASCII by adding 0x30
    x3 = 0xDF; // ASCII value of degree(°) symbol

    datawrt(x1);  // Display temperature on 16x2 LCD display
    datawrt(x2);
    datawrt(x3);
    datawrt('C');
}

void delay(unsigned int z) {
    unsigned int p, q;
    for (p = 0; p < z; p++) { // Repeat for 'z' times
        for (q = 0; q < 1375; q++); // Repeat for 1375 times
    }
}
