/*****************************************************************************
 * Copyright (C) 2024 by Lokesh Senthil Kumar
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users
 * are permitted to modify this and use it to learn about the field of
 * embedded software. Lokesh Senthil Kumar and the University of Colorado are not 
 * liable for any misuse of this material.
 *****************************************************************************/

/**
 * @file    cone.c
 * @brief   File for single-step execution debugger monitor program.
 * @details This program implements single-step execution for user code in 
 *          the AT89C51RC2 microcontroller. Using INT1 (configured as level-triggered),
 *          the debugger monitors and prints the updated register values 
 *          (ACC, B, PSW, SP, DPTR, R0-R7, PC) after each instruction execution.
 *          It also provides UART-based interaction for user commands and address entry.
 * @date    December 14, 2024
 * @version 1.0
 */

#include <REG51.H>
#include <stdio.h>

//Declarations and prototype
void uart_init();
void jump_to_user_code();
void trans_string(const char *str);
void jump(void);
void hex(void);
int c;
char z;
unsigned char pc_low;
unsigned char pc_high;
char accbuffer[10];
char bbuffer[10];
char pswbuffer[10];
char buffer[10];
unsigned char *sp_addr;    
unsigned int pc_value;
unsigned int lastpc;
unsigned int flagy;
unsigned int FL;
unsigned int dpl;
unsigned int dph;
unsigned char sp_val, acc_value, b_value, psw_value;
unsigned int dptr_value;
unsigned char r_values[8];
unsigned char exit;
int i;
int j;
unsigned char hex_string[5]; // Buffer for the 4-digit hex string
unsigned int address = 0;
int digit;
unsigned int user_address;
void (*user_code)(void);
char cmd;

/**
 * @brief   Initializes UART communication at 9600 baud rate.
 * @details Configures Timer 1 in mode 2 for auto-reload, enabling 8-bit UART mode.
 * @param   None
 * @return  None
 */
void uart_init(void) 
{
    TMOD |= 0x20;               // Set Timer 1 in Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;                 // Set reload value for 9600 baud rate (11.0592 MHz clock)
    SCON = 0x50;                // 8-bit UART mode, REN enabled (Receive Enable)
    TR1 = 1;                    // Start Timer 1
    TI = 1;                     // Set TI to indicate transmitter is ready
}

/**
 * @brief   Sends a single character via UART.
 * @details Blocks until the character is transmitted via the SBUF register.
 * @param   c - The character to be transmitted.
 * @return  None
 */
void trans (char c)
{
		SBUF = c;
    while (!TI);
    TI = 0;
		
}

/**
 * @brief   Receives a single character via UART.
 * @details Blocks until a character is received via the SBUF register.
 * @param   None
 * @return  The received character.
 */
char typeit (void){
    while (!RI);
    RI = 0;
    return SBUF;
}
	
/**
 * @brief   Prompts the user to enter a 4-digit hexadecimal address.
 * @details Accepts and validates user input to convert it into an address.
 *          Allows backspace corrections during input.
 * @param   None
 * @return  The hexadecimal address as an unsigned integer.
 */
unsigned int get_user_address()
{
		unsigned char bp;
		flagy = 1;
    trans_string("\n\n\r Enter the address: ");
    for (i = 0; i < 4; i++) {
				bp = typeit();
				if (bp == '\b'){
					trans('\b');   // Send Backspace to move cursor back
					trans(' ');    // Send Space to overwrite the character
					trans('\b');
					i=i-2;
				} else {
        hex_string[i] = bp; // Get each hex character
        trans(hex_string[i]);     // Echo back the character
				}
    }
    hex_string[4] = '\0'; // Null-terminate the string

    // Convert the hex string to an integer value
    for (i = 0; i < 4; i++) {
        if (hex_string[i] >= '0' && hex_string[i] <= '9') {
            digit = hex_string[i] - '0';
        } else if (hex_string[i] >= 'A' && hex_string[i] <= 'F') {
            digit = hex_string[i] - 'A' + 10;
        } else if (hex_string[i] >= 'a' && hex_string[i] <= 'f') {
            digit = hex_string[i] - 'a' + 10;
        } else {
            // Invalid input, default to 0
            digit = 0;
        }
        address = (address << 4) | digit; // Shift and add the digit
    }
    return address;
}

/**
 * @brief   Initiates single-step execution of user code.
 * @details Jumps to user-specified code address and executes instructions 
 *          step-by-step. On every INT1 interrupt, the register values are 
 *          displayed via UART.
 * @param   None
 * @return  None
 */
void jump_to_user_code() {
    if (!(P3 & 0x08)){
    user_address = get_user_address();
    user_code= (void (*)(void))user_address; 
	trans_string("\r\n\n Single-Step execution started: Press 'E' to exit!\r\n");
	trans_string("\r\n -----------------------------------------------\r\n");
    IT1 = 0;
    EX1 = 1;                 
    EA = 1;         
    user_code();  
		} else {
			trans_string("P3.3 (INT1) is not connected to low!");
		}
}

/**
 * @brief   Interrupt handler for INT1 to capture and display register values.
 * @details Captures the stack pointer, program counter, and register values 
 *          (ACC, B, PSW, DPTR, R0-R7) after each user instruction execution.
 *          Provides UART-based interaction for exiting single-step mode.
 * @param   None
 * @return  None
 */

void int1_handler() interrupt 2 {
	
		for (i = 0; i < 100; i++){
			for (i = 0; i < 100; i++){}
		}
		TI = 1;
		
		
    // Get Stack Pointer (SP)
    sp_val = SP;
    sp_addr = (unsigned char *)sp_val;
		for (i = 7; i >= 0; i--) {
        r_values[i] = *((unsigned char *)sp_addr);
				sp_addr--;// General-purpose registers R0-R7
    }
		
		psw_value = *((unsigned char *)sp_addr);   // Program Status Word
		sp_addr--;
		
		dpl = *((unsigned char *)sp_addr);
		sp_addr--;
		dph = *((unsigned char *)sp_addr);
		sp_addr--;
		dptr_value = (dpl | (dph << 8)); // Data Pointer
		
		b_value = *((unsigned char *)sp_addr);
		sp_addr--;
		
		acc_value = *((unsigned char *)sp_addr);
		sp_addr--;
		
    // Get Program Counter (PC)
		pc_high = *((unsigned char *)sp_addr);
		sp_addr--;
    pc_low = *((unsigned char *)sp_addr);
    sp_addr--;
		
		if (flagy){
			pc_value = user_address;
			flagy = 0;
		} else {
			pc_value = lastpc;
		}
		lastpc = (pc_high << 8) | pc_low;
    //pc_value = (pc_high << 8) | pc_low;
		
		sp_val = (unsigned char)sp_addr;
		
		if (FL){
		
		trans_string("\n\r ACC B PSW DPTR R0 R1 R2 R3 R4 R5 R6 R7 SP PC\n\r ");
    
    sprintf(accbuffer, "%04X ", acc_value);
    for (i = 0; i < 2; i++) trans(accbuffer[i]);
		trans(' ');
    sprintf(bbuffer, "%04X ", b_value);
    for (i = 0; i < 2; i++) trans(bbuffer[i]);
		trans(' ');
    sprintf(pswbuffer, "%04X ", psw_value);
    for (i = 2; i < 4; i++) trans(pswbuffer[i]);
		trans(' ');
    sprintf(buffer, " %04X ", dptr_value);
    for (i = 0; buffer[i] != '\0'; i++) trans(buffer[i]);

    // Print R0-R7
    for (i = 0; i < 8; i++) {
        sprintf(buffer, "%02X ", r_values[i]);
        for (j = 0; j < 2 ; j++) trans(buffer[j]);
				trans(' ');
    }

    // Print SP
    sprintf(buffer, "%02X ", sp_val);
    for (i = 0; i < 2; i++) trans(buffer[i]);
		

    // Print PC
    sprintf(buffer, " %04X", pc_value);
    for (i = 0; buffer[i] != '\0'; i++) trans(buffer[i]);

		
		while (1){
			exit = typeit();
			if (exit == 'E'){
			trans('\n');
			trans('\r');
			EX1 = 0;                 
			EA = 0;
			trans_string(" -----------------------------------------------\r\n");
			//exity();
			break;
			} else if ( exit == '\r'){
					trans('\n');
					trans('\n');
					trans('\r');
					break;
				}
		}
    
	} else if (lastpc == user_address){
			FL = 1;
	}
    TI = 1;
}

/**
 * @brief   Sends a null-terminated string via UART.
 * @details Loops through the string and transmits each character.
 * @param   str - Pointer to the string to be transmitted.
 * @return  None
 */
void trans_string(const char *str)
{
    i = 0;
    while (str[i] != '\0') // Loop until the null terminator is encountered
    {
        trans(str[i]);     // Call trans() for each character
        i++;
    }
}

/**
 * @brief   Displays a help menu with available debugger commands.
 * @details Lists commands for memory editing, single-step execution, 
 *          and jumping to user code.
 * @param   None
 * @return  None
 */
void help(void){
	
	 trans_string("\r\n\n ====================================\r\n\n");
    trans_string("         AT89C51RC2 Debugger Monitor\r\n\n");
    trans_string(" ====================================\r\n\n");
    trans_string(" Available Commands:\r\n\n");
    trans_string(" M - Memory Editor\r\n\n");
    trans_string(" S - Single Step Execution\r\n\n");
    trans_string(" J - Jump to User code\r\n\n");
    trans_string(" H - Display This Help Menu\r\n\n");
    trans_string(" ====================================\r\n\n");

}

/**
 * @brief   Executes user-specified code without interrupt monitoring.
 * @details Directly jumps to the user-provided address to execute code.
 * @param   None
 * @return  None
 */
void jump(void){
		user_address = get_user_address();
    user_code= (void (*)(void))user_address; 
		user_code();
}

/**
 * @brief   Executes a memory hexdump starting at address 0x2000.
 * @details Performs a memory dump by calling the hexdump function 
 *          located at the specific address.
 * @param   None
 * @return  None
 */
void hex(void){
	void (*hexdump)(void) = (void (*)(void))0x2000;
	hexdump();
}


void main() {
    uart_init();
		while(1){
		help();
    trans_string("\r\n Enter the Command: ");
    cmd = typeit();
    trans(cmd);
		trans_string("\033[2J\033[H");
    switch (cmd) {
				case 'M': case 'm':
            hex();
            break;
        case 'S': case 's':
            jump_to_user_code();
            break;
        case 'H': case 'h':
            break;
				case 'J': case 'j':
            jump();
            break;
        default:
            trans_string("\r\n Invalid Command !\r\n");
            break;
    }
	}
}
