#include <msp430.h>

volatile long lightRaw;
volatile long tempRaw;
char result[100];
volatile long sample[100];

void ConfigureAdc_light();
void ConfigureAdc_temp();

void uart_init(void);
void ConfigClocks(void);
void strreverse(char *begin, char *end);
void itoa(int value, char *str, int base);
void port_init();

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    port_init();
    ConfigClocks();
    uart_init();
    _delay_cycles(5);                // Wait for ADC Ref to settle

    while (1)
    {
        int counter = 0;
        if (counter == 0)
        {
            _delay_cycles(20000000); // Delay for about 1 second 

            int acount = 0;
            counter++;

        }

        if (counter == 1)
        {
            // Temperature sensor setup and transmission
            ConfigureAdc_temp();
            ADC10CTL0 |= ENC + ADC10SC + MSC; // Converter Enable, Sampling/conversion start
            while ((ADC10CTL0 & ADC10IFG) == 0) // Check the Flag, while its low just wait
                P1OUT |= BIT6;                         // Green LED on
            tempRaw = ADC10MEM;       // Read the converted data into a variable
            P1OUT &= ~BIT6;                        // Green LED off
            ADC10CTL0 &= ~ADC10IFG;                // Clear the flag
            // Convert temperature into usable data 
            itoa(((tempRaw * 48724 - 30634388) >> 16), result, 10);

            int acount = 0;

            while (result[acount] != '\0')
            {
                while ((IFG2 & UCA0TXIFG) == 0)
                {
                    //Wait Uuntil the UART transmitter is ready
                    UCA0TXBUF = result[acount++];
                }
            }
            counter++;
        }

        if (counter == 2)
        {
            // Light sensor setup and transmission
            ConfigureAdc_light();
            ADC10CTL0 |= ENC + ADC10SC + MSC; // Converter Enable, Sampling/conversion start
            P1OUT |= BIT6; // Green LED on
            while ((ADC10CTL0 & ADC10IFG) == 0) // Check the Flag, while its low just wait
                _delay_cycles(20000000); // Delay 
            lightRaw = ADC10MEM; // Read the converted data into a variable
            P1OUT &= ~BIT6;   // Green LED off
            ADC10CTL0 &= ~ADC10IFG;
            / Clear the flag
            itoa(lightRaw, result, 10);

            int acount = 0;

            while (result[acount] != '\0')
            {
                while ((IFG2 & UCA0TXIFG) == 0)
                {
                    //Wait Until the UART transmitter is ready
                    UCA0TXBUF = result[acount++];
                }
            }
            counter = 0;

        }

    }
}

// Configure ADC Temperature
void ConfigureAdc_temp()
{
    ADC10CTL1 = INCH_10 + ADC10DIV_0 + CONSEQ_2;
    ADC10CTL0 = SREF_1 | ADC10SHT_3 | REFON | ADC10ON;
//Vref+, Vss, 64 ATD clocks per sample, internal references, turn ADCON
    __delay_cycles(5);                              //Wait for ADC Ref to settle
    ADC10CTL0 |= ENC | MSC; //Converter Enable, Sampling/Conversion start, multiple sample/conversion operations
}

//Configure ADC  Light
void ConfigureAdc_light()
{
    ADC10CTL1 = INCH_6 + ADC10DIV_0 + CONSEQ_2;
    ADC10CTL0 = SREF_1 | ADC10SHT_3 | REFON | ADC10ON;
//Vref+, Vss, 64 ATD clocks per sample, internal references, turn ADCON
    __delay_cycles(5);                              //wait for ADC Ref to settle
    ADC10CTL0 |= ENC | MSC; //Converter Enable, Sampling/Conversion start, multiple sample/conversion operations
}

void uart_init(void)
{
    UCA0CTL1 |= UCSWRST;                     //Disable the UART state machine
    UCA0CTL1 |= UCSSEL_3;       //Select SMCLK as the baud rate generator source
    UCA0BR1 = 0;
    UCA0BR0 = 104;                           //Produce a 9,600 Baud UART rate
    UCA0MCTL = 0x02;        //Choose appropriately from Table 15-4 in User Guide
    UCA0CTL1 &= ~UCSWRST;                    //Enable the UART state 
    IE2 |= UCA0RXIE;                        //Enable the UART receiver Interrupt
}
void ConfigClocks(void)
{

    BCSCTL1 = CALBC1_1MHZ;                     // Set range
    DCOCTL = CALDCO_1MHZ;                      // Set DCO step + modulation
    BCSCTL3 |= LFXT1S_2;                       // LFXT1 = VLO
    IFG1 &= ~OFIFG;                            // Clear OSCFault flag
    BCSCTL2 = 0;                               // MCLK = DCO = SMCLK
}

void strreverse(char *begin, char *end) // Function to reverse the order of the ASCII char array elements
{
    char aux;
    while (end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

void itoa(int value, char *str, int base)
{  //Function to convert the signed into an ASCII char array

    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;
    int sign;

    // Validate that base is between 2 and 35 (inclusive)
    if (base < 2 || base > 35)
    {
        *wstr = '\0';
        return;
    }

    // Get magnitude and the value
    sign = value;
    if (sign < 0)
        value = -value;

    do                      // Perform integer-to-string conversion.
        *wstr++ = num[value % base]; // Create the next number in converse by taking mod 
    while (value /= base);     // stop when you get  a 0 for the quotient

    if (sign < 0)              // Attach sign character, if needed
        *wstr++ = '-';
    *wstr = '\0'; // Attach a null character at end of char array. The string is in reverse order at this point
    strreverse(str, wstr - 1); // Reverse string

}

void port_init()
{
    P1SEL |= BIT1 + BIT2;            // select non-GPIO  usage for Pins 1 and 2
    P1SEL2 |= BIT1 + BIT2;           // Select UART usage of Pins 1 and 2
}
