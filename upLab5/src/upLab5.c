// #include <stdio.h>
// #include <stdint.h>
// #include <sleep.h>

// // define the addresses for the GPIO inputs directly
// #define Bank0_Input *((uint32_t *)0xE000A060) // bank0 inputs
// #define Bank2_Input *((uint32_t *)0xE000A068) // bank2 inputs
// // the GPIO BANK output data
// #define Bank0_Output *((uint32_t *)0xE000A040) // bank0 inputs
// #define Bank2_Output *((uint32_t *)0xE000A048) // bank2 inputs
// // the GPIO BANK Direction data
// #define Bank0_Dir *((uint32_t *)0xE000A204) // bank0 inputs
// #define Bank2_Dir *((uint32_t *)0xE000A284) // bank2 inputs
// // the GPIO BANK Enable data
// #define Bank0_Enable *((uint32_t *)0xE000A208) // bank0 inputs
// #define Bank2_Enable *((uint32_t *)0xE000A288) // bank2 inputs

// #define SW_DATA *((uint32_t*)0x41220000) // switch register
// #define M_SW 0x01 // gets the first bit, which reads the status of switch0

// #define M_OFF ~(0x70000) // mask where r,g,b bits are all 0.
// #define M_RED (1 << 17) // mask for red bit
// #define M_GREEN (1 << 18) // mask for green bit
// #define M_YELLOW 0x60000 // mask for both red and green bits (creates yellow)

// // communication protocol definitions
// #define COMM_BITS    0x30 // grabs bottom 2 bits needed for comm protocol
// #define OFF     0b00//0b0001
// #define RED     0b01//0b0010
// #define GREEN   0b10//0b0100
// #define YELLOW  0b11//0b1000

// #define LD 2 // long delay for red and green lights
// #define SD 1 // short delay for yellow light

// // sets the direction and enables the PMOD C pins
// // 1-4 are set to be outputs
// // 7-10 are set to be inputs
// // code by me, helped with logic by partner Eric.
// void PMODC_enable(){
//     // Direction
//     uint32_t temp = Bank2_Dir;
//     temp |= (0x0F << 15); // set needed bits to 1 for output
//     Bank2_Dir = temp;
    
//     // Enable
//     temp = Bank2_Enable;
//     temp |= ~(COMM_BITS << 15); // write 0 to the needed bits to enable
//     Bank2_Enable = temp;    
// }

// // writes the color signal to the correct place in bank2, 
// // given an "output", which is the color signal. 
// void PMODC_write(uint32_t output){
//     uint32_t temp = Bank2_Output;
//     temp &= ~(0x0F << 15); // clear the needed bits
//     temp |= ((output & 0x0F) << 15); // grab bottom 4 bits and place output into position
//     Bank2_Output = temp;
// }

// // reads bank2 for color signal and returns the 4 signal bits.
// uint32_t PMODC_read(){
//     // int test = Bank2_Input;
//     return ((Bank2_Input & (COMM_BITS << 15)) >> (15+4)); // 0x30 grabs only the two bits we're looking at (if using all 4 bits, use 0xF0)
// }

// // sets the direction and enables the bank0 LD12 pins
// // 1-4 are set to be outputs
// // 7-10 are set to be inputs
// // code by me, helped with logic by partner Eric.
// void rgbLED_enable(){
//     // Direction
//     uint32_t temp = Bank0_Dir;
//     temp |= (0x0F << 15); // set needed bits to 1 for output
//     Bank0_Dir = temp;
    
//     // Enable
//     temp = Bank0_Enable;
//     temp |= ~(COMM_BITS << 15); // write 0 to the needed bits to enable
//     Bank0_Enable = temp;  
// }

// // returns the mask for the desired color
// // signaling byte: 0b[yellow][green][red][no colors]
// uint32_t decode_signal(uint32_t colorSignal){
//     switch(colorSignal){
//         case YELLOW:
//             return M_YELLOW;
//         case GREEN:
//             return M_GREEN;
//         case RED:
//             return M_RED;
//         case OFF:
//             return M_OFF;
//     }
//     return 0;
// }

// // turns off r,g,b bits. 
// void clearLD12(void){
//     Bank0_Output &= M_OFF;
// }

// // writes to the desired LD12 bits in bank0
// void rgbLED_write(uint32_t colorMask){
//     uint32_t temp = Bank0_Output;
//     temp &= M_OFF; // clear r,g,b bits
//     temp |= colorMask; // turn on bits as dictated by color mask
//     Bank0_Output = temp;
// }

// // enables and sets directions for PMOD C pins and bank0 rgbLED pins
// // clears the LED pins
// void gpio_enable(void){
//     PMODC_enable();
//     rgbLED_enable();
//     clearLD12();
// }

// // Uses PMODC_write to send signal with desired color to E/W board,
// // and uses rgbLED_write to write to bank0 on N/S board to turn on LD12.
// // Performs this with the proper timing delays. 
// void north_south_master(void){
//     rgbLED_write(M_RED);
//     while(SW_DATA & M_SW){ // when switch 0 is on, this board is N/S
//         PMODC_write(RED); 
//         sleep(LD);
//         rgbLED_write(M_GREEN);
//         sleep(LD);
//         rgbLED_write(M_YELLOW);
//         sleep(SD);
//         rgbLED_write(M_RED);
//         sleep(LD);
//         PMODC_write(GREEN);
//         sleep(LD);
//         PMODC_write(YELLOW);
//         sleep(SD);
//     }
// }

// // runs a while loop as long as switch0 is off. 
// // continually reads input pins and writes what it sees to LD12. 
// void east_west_slave(void){
//     uint32_t inputSignal;
//     while(~(SW_DATA & M_SW)){
//         inputSignal = PMODC_read();
//         rgbLED_write(decode_signal(inputSignal));
//     }
// }

// void east_west_slave_temp(void){
//     while(!(SW_DATA & M_SW)){
//         rgbLED_write(M_RED);
//         sleep(LD);
//         clearLD12();
//         sleep(SD);
//         rgbLED_write(M_YELLOW);
//         sleep(LD);
//         clearLD12();
//         sleep(SD);
//         rgbLED_write(M_GREEN);
//         sleep(LD);
//         clearLD12();
//         sleep(SD);
//     }
// }

// // testing function
// void RED_BLUE_works(void){
//     uint32_t temp = Bank0_Output;
//     rgbLED_enable();
//     temp &= ~(0x38000); // clear the rgb bits
//     Bank0_Output = temp;
//     sleep(1);
//     temp |= (1 << 17); // red
//     Bank0_Output = temp;
//     sleep(1);
//     temp &= ~(0x38000); // clear the rgb bits
//     temp |= (1 << 16); // blue
//     Bank0_Output = temp;
//     sleep(2);
//     temp |= (1 << 17); // add red to blue for magenta
//     Bank0_Output = temp;
// }

// ////////////////////////////////////////

// int main(void){
//     gpio_enable();
//     while(1){
//         if(SW_DATA & M_SW){
//             north_south_master();
//         } else{
//             east_west_slave();
//         }
//     }

//     return 1;
// }

// // red is bit 17. that's the 18th bit. so 1 << 17
// // green is bit 18. so 1 << 18
// // blue is bit 16. so 1 << 16
// // for all three, we need 0111 0000 0000 0000 0000
// // so 0x70000
// // yellow is green + red at same time. we need 0110 0000 0000 0000 0000
// // so 0x60000


// Microprocessors Lab 4
// UART "console" communication
// Prime Number Finder

#include <stdint.h>
#include <stdio.h>

#define UART1_CR        *((uint32_t *) 0xE0001000) // UART1 control register
#define UART1_MR        *((uint32_t *) 0xE0001004) // UART1 Mode register
#define UART1_BAUDGEN   *((uint32_t *) 0xE0001018)
#define UART1_BAUDDIV   *((uint32_t *) 0xE0001034)

#define UART1_SR        *((uint32_t *) 0xE000102C) //UART1 status reg
#define UART1_DATA      *((uint32_t *) 0xE0001030) //UART1 TX/RX FIFO DATA
#define BaudGen115200 0x7c 
#define BaudDiv115200 6

// Welker function
int uart1_tx_full(){
    return (UART1_SR & 16) != 0; // check sr register bit 4, return 1 if fifo full
}

// Welker function
// returns 0 if rx FIFO not empty
// returns 1 if rx FIFO is empty
int uart1_rx_empty(){
    uint32_t uartData; // general variable to hold data to/from UART
    uartData = UART1_SR;
    // 0x02 mask checks "Receiver FIFO Empty Continuous Status" bit. 
    // if this bit is 0, Rx FIFO is not empty
    // if htis bit is a 1, Rx FIFO is empty
    if((uartData & 0x02) == 0b10){ // == 2
        return 1; // rx FIFO is empty
    } else {
        return 0;
    }
}

// Welker function
void reset_uart1(){
    UART1_CR = 3; // toggle last two bits in control reg to reset logic //|= ?
    while((UART1_CR & 0x02) == 2){} // wait till xmit reset clears
    while((UART1_CR & 0x01) == 1){} // wait till rcv reset clears
}

// Welker function
// sets baudrate to 115200
void setBaudrate1(){
    UART1_BAUDGEN = BaudGen115200;  // 0x7C = 124
    UART1_BAUDDIV = BaudDiv115200;  // 6
}

// Welker function
// configures UART to 8N1
void configure_uart1(){
    uint32_t uart_data; // general variable to hold data to/from UART
    UART1_MR = 0x20; // toggles bit 5 on, which sets it to no parity
    uart_data = UART1_CR; //get the current setting and OR in the enabling bits
    uart_data |= 0x14; // enable the TX (bit 2) and RX (bit 4) via OR 0x14 to control register
    UART1_CR = uart_data;
}

// Welker function
// set to 115200, 8N1
void init_uart1(){
    reset_uart1(); // reset the UART
    configure_uart1(); // configure to 8N1
    setBaudrate1(); // set the baudrate to 115200
}

void configure_uart1_v2(){
    // CR bits: [5]=TX_DIS, [3]=RX_DIS — set these to disable
    UART1_CR = 0x28; // i dont remember why I made this. This is the only difference.
    
    uint32_t UARTData; // general variable to hold data to/from UART
    UART1_MR = 0x20; // toggles bit 5 on, which sets it to no parity
    // enable the transmit and recieve via OR 0x14 to control register
    UARTData = UART1_CR; //get the current setting and OR in the enabling bits
    UARTData |= 0x14; // Enable TX and RX
    UART1_CR = UARTData;
}

void transmit_char(char out){
    uint32_t output = (uint32_t) out; // change to 32 bit integer
    while(uart1_tx_full()){} // wait for the tx fifo to be empty
    UART1_DATA = output; // write input into the DATA register
}

char receive_char(){
    while(uart1_rx_empty()){} // wait for the rx fifo to be empty
    return (char)UART1_DATA; // cast the integer from the DATA register to a char to be returned by this function
}

void uart1_send_string(char *str){
    // step through the string array and transmit each char
    for(int i = 0; str[i] != '\0'; i++){
        transmit_char(str[i]);
    }
}

// takes a string made of numbers (i.e. "1234") and returns the number
// as an integer 1234
uint32_t ascii_to_int(char *ascii){
    uint32_t target = 0; // start with 0
    // ascii integer codes in hex are 0x30-0x39 for 0-9.
    // hence subtracting 0x30 from the next value in the array
    for(int i = 0; ascii[i] != '\0'; i++){
        target *= 10; // move to the next order of magnitude
        target += (ascii[i] - 0x30); // add the integer value to the target
    }
    return target;
}

// read the value in the DATA register into a char[]
// echo the user's inputted chars back to the console for the user to see
// convert the char[] into an integer
// returns the user input as an integer.
// returns 0 if the user input was negative.
uint32_t read_int_from_user(){
    char store_input[16] = {1}; // creates a blank char array to store the user's input. {1} clears the junk bits
    char input; // char used to store value read from the UART RX FIFO
    uint32_t i = 0;
    
    while(1){
        input = receive_char(); // get the next char in the string
        
        if(input == '-'){
            transmit_char('\n'); // send a newline for aesthetic purposes.
            return 0; // if the user input is negative, return 0.
        }
        
        if(input == '\r' || input == '\n'){ // first check if end of string by looking for return / newline chars
            uart1_send_string("\r\n"); // transmit newline char to console, expected by windows
            break; // exit loop
        }

        store_input[i] = input; // store the current input char into the store_input array
        i++; // increment i to store next char in the next pos in the array
    }
    
    i++; // move into last needed slot in array
    store_input[i] = '\0'; // add null terminator to end the string
    
    return ascii_to_int(store_input); // convert store_input array to integer and return it
}

// user should input a positive integer greater than or equal to 2
// returns a 1 if the user input is valid
// returns a 0 if the user input is invalid
int validate_input(uint32_t user_int){
    if(user_int >= 2){
        return 1;
    }
    return 0;
}



// determine a prime number by dividing by every odd number up to sqrt(num)
// if divisible evenly by any of those numbers, it is not prime.
// if divisible by number > sqrt(num), then must be divisible by number < sqrt(num). Hence stop search here.
// if the number is not prime, decrement and run the test again until a prime is found.
// returns the first prime integer found.
uint32_t find_prime(uint32_t num){
    // 2 is only even prime
    if(num == 2){
        return num;
    }
    // even numbers > 2 are not prime
    if(num % 2 == 0){
        return find_prime(num-1);
    }
    
    // check all odd divisors up till the sqrt
    for(uint32_t i = 3; i*i <= num; i += 2){
        if(num % i == 0){
            return find_prime(num-1);
        }
    }
    
    return num;
}

int main(void){
    init_uart1();
    uint32_t user_int;
    
    uart1_send_string("\n\n   Microprocessors Lab 4 - Seth L\n-~================================~-\n");

    while(1){
        uart1_send_string("\nEnter a positive integer greater than or equal to 2: ");

        user_int = read_int_from_user();
        while(!(validate_input(user_int))){ // wait for the user to enter a valid number
            uart1_send_string("\nThat input was invalid, please try again.\nEnter a positive integer greater than or equal to 2: ");
            user_int = read_int_from_user();
        }

        char ascii_from_int[16];
        sprintf(ascii_from_int, "%d", find_prime(user_int));
        
        uart1_send_string("\nThe nearest prime number found is ");
        uart1_send_string(ascii_from_int);
        uart1_send_string("\n\n");
    }
    
    return 1;
}

