/*****************************************************************************
 * Copyright (C) 2024 by Bhavya Saravanan
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users
 * are permitted to modify this and use it to learn about the field of
 * embedded software. Bhavya Saravanan and the University of Colorado are not 
 * liable for any misuse of this material.
 *****************************************************************************/

/**
 * @file    main.C
 * @brief   Main file for a simple user program located at address 0x4000.
 * @details Provides UART initialization and character I/O functions. The program
 *          verifies ACC and B register updates through single-step execution.
 * @date    December 14, 2024
 * @version 1.0
 */
#include <at89c51ed2.h>
#include <stdio.h>
#include <stdlib.h>

  // Function prototype
int putchar(int c);       
int getchar(void);        


/**
 * @brief   Initializes UART communication at 9600 baud rate.
 * @param   None
 * @return  None
 */
void uart_init(void) {
    TMOD |= 0x20;               // Set Timer 1 in Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;                 // Set reload value for 9600 baud rate (11.0592 MHz clock)
    SCON = 0x50;                // 8-bit UART mode, REN enabled (Receive Enable)
    TR1 = 1;                    // Start Timer 1
    TI = 1;                     // Set TI to indicate transmitter is ready
}

/**
 * @brief   Transmits a single character via UART.
 * @param   c - The character to be transmitted.
 * @return  The transmitted character.
 */
int putchar(int c) {
    SBUF = c;                   // Load the character into the UART buffer (SBUF)
    while (!TI);                // Wait for transmission to complete (TI flag set)
    TI = 0;                     // Clear the TI flag for the next transmission
    return c;                   // Return the transmitted character
}

/**
 * @brief   Receives a single character via UART.
 * @param   None
 * @return  The received character.
 */
int getchar(void) {
    while (!RI);                // Wait for a character to be received (RI flag set)
    RI = 0;                     // Clear the RI flag for the next reception
    return SBUF;                // Return the received character from the UART buffer (SBUF)
}


void main(void) {
    uart_init();
    while(1){
        ACC = 0x11;
        B = 0x22;
        printf("\r\nHey everyone");
        for (int i = 0; i < 100; i++){
            for (int j = 0; j <100; j++){}
        }
    }
}