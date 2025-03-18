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
 * @file    code_memory.c
 * @brief   Implements code memory read functionality.
 * @details Provides utility functions to read and parse memory content from 
 *          specific address ranges in the microcontroller's code memory.
 * @author  Bhavya Saravanan
 * @date    December 14, 2024
 * @version 1.0
 */


#include <at89c51ed2.h>
#include <stdio.h>
#include <stdint.h>
#include "code_memory.h"
#include "xram_memory.h"
#define MAX_DIGITS 5
#define CARRIAGE_RETURN 13
#define BACKSPACE 8
#define SPACE 32

#define ENTER_KEY (13)
#define DIVIDE_BY_16 (16)
#define ASCII_SPACE (32)
#define NUMBER_BASE (16)
#define DATA_MAX (255)
#define ADDRESS_MAX (0x7FFF)
#define CODE_START_ADDRESS (0x0000)

char int_to_char(int num);
unsigned char char_to_int(unsigned char ch);
unsigned int parse_user_input(unsigned char base);
void read_code_memory(void);
void code_memory_read(unsigned int start_address, unsigned int end_address);
void print_hex_number(uint32_t num, unsigned char width);


char int_to_char(int num) {
    if (num >= 0 && num <= 9)
        return '0' + num;
    else if (num >= 10 && num <= 15)
        return 'A' + (num - 10);
    return '0';
}

unsigned char char_to_int(unsigned char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return 0;
}

unsigned int parse_user_input(unsigned char base) {
    unsigned int number = 0;
    unsigned int digits[MAX_DIGITS] = {0};
    unsigned int digit_count = 0;
    unsigned char current_char = 0;

    while (current_char != CARRIAGE_RETURN) {
        current_char = getchar();
        if (((current_char >= '0') && (current_char <= '9')) ||
            ((current_char >= 'a') && (current_char <= 'f')) ||
            ((current_char >= 'A') && (current_char <= 'F'))) {
            putchar(current_char);
            digits[digit_count++] = char_to_int(current_char);
        } else if (current_char == BACKSPACE && digit_count > 0) {
            putchar(BACKSPACE);
            putchar(SPACE);
            putchar(BACKSPACE);
            digit_count--;
        }
    }

    for (unsigned int i = 0; i < digit_count; i++) {
        number = number * base + digits[i];
    }
    return number;
}

void print_hex_number(uint32_t num, unsigned char width) {
    unsigned char buffer[10];
    int8_t digit_count = 0;
    do {
        buffer[digit_count++] = int_to_char(num % 16);
        num /= 16;
    } while (num > 0);

    while (digit_count < width) {
        putchar('0');
        width--;
    }

    for (int i = digit_count - 1; i >= 0; i--) {
        putchar(buffer[i]);
    }
}
void read_code_memory(void) {
    unsigned int start_address;
    unsigned int end_address;

    printf("\r\n Enter Start Address (Code Memory): ");
     //printf("\r\n");
    start_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    if (start_address < CODE_START_ADDRESS) {
        printf("\r\n Invalid Code Memory Start Address. Must be >= 0x0000.\r\n");
        return;
    }

    printf("\r\n Enter End Address (Code Memory): ");
     //printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    if (end_address < CODE_START_ADDRESS) {
        printf("\r\n Invalid Code Memory End Address. Must be >= 0x0000.\r\n");
        return;
    }
    printf("\r\n-----------------------CODE MEMORY CONTENTS----------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");
    code_memory_read(start_address, end_address);
    printf("\r\n-------------------------------------------------------------------\r\n");

    
}

void code_memory_read(unsigned int start_address, unsigned int end_address) {
    unsigned char count = 0;
    unsigned char __code *ptr;

    for (unsigned int addr = start_address; addr <= end_address; addr++) {
        ptr = (unsigned char __code *)addr;

        // Start a new row for every 16 addresses
        if (count % DIVIDE_BY_16 == 0) {
            putchar('\n');
            putchar('\r');

            print_hex_number(addr, 4);
            printf(": ");
        }

        // Print the code memory content
        print_hex_number(*ptr, 2);
        printf("  "); // Add spacing between numbers

        count++;
    }

    
       
        printf("\r\n");
    
}
  