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

