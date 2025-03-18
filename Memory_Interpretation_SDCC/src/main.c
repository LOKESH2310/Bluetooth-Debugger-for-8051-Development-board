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
 * @file    main.c
 * @brief   Entry point for the memory editor application.
 * @details Provides UART initialization, user command handling, and menu-based 
 *          memory access operations for code and XRAM memory regions.
 * @author  Bhavya Saravanan
 * @date    December 14, 2024
 * @version 1.0
 */


#include <at89c51ed2.h>
#include <stdio.h>
#include <stdint.h>
#include "code_memory.h"
#include "xram_memory.h"

// Function Prototypes
void display_help(void);
int getchar(void);
int putchar(int c);
void uart_initialization(void);
void handle_command(void);

void display_help(void) {
    printf("\r\n========================================\r\n");
    printf("\r\n");
    printf("         MEMORY EDITOR \r\n");
    printf("\r\n");
    printf("============================================\r\n");
    printf("\r\n");
    printf(" Available Commands:\r\n");
    printf("\r\n");
    printf("< R >  Read Data Memory\r\n");
    printf("\r\n");
    printf("< W >  Write Data Memory\r\n");
    printf("\r\n");
    printf("< C >  Read Code Memory\r\n");
    printf("\r\n");
    printf("< H >  Display This Help Menu\r\n");
    printf("\r\n");
    printf("< X >  Exit \r\n");
    printf("\r\n");
    printf("===========================================\r\n");
}

int getchar(void) {
    while (!RI);
    RI = 0;
    return SBUF;
}

int putchar(int c) {
    while (!TI);
    SBUF = c;
    TI = 0;
    return c;
}

void uart_initialization(void) {
    TMOD |= 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TR1 = 1;
    ES = 1;
    EA = 1;
    TI = 1;
}

void handle_command(void) {
    char cmd;
    printf("\r\nEnter Command (H for help): ");
    cmd = getchar();
    putchar(cmd);
    printf("\r\n");

    switch (cmd) {
        case 'R':
        case 'r':
            read_memory();
            break;
        case 'W':
        case 'w':
            write_memory();
            break;
        case 'C':
        case 'c':
            read_code_memory();
            break;
        case 'H':
        case 'h':
            display_help();
            break;
        case 'X':
        case 'x': {
             printf("\033[2J\033[H"); // Clear screen and reset cursor position
            printf("\r\nExiting to 0x0000...\r\n");

            // Reset to 0x0000
            void (*reset_to_0x0000)(void) = (void (*)(void))0x0000;
            reset_to_0x0000();
            break;
        }
        default:
            printf("\r\n Invalid Command. Press 'H' for help.\r\n");
            break;
    }
    printf("\r\n ******************************************************\r\n");
}


void main(void) {
    uart_initialization();
    display_help();
    initialize_xram();
    

    while (1) {
        handle_command();
    }
}