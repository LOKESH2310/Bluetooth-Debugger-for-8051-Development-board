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
 * @file    xram_memory.c
 * @brief   Implements XRAM memory read/write functionality.
 * @details Provides functions for initializing XRAM, reading data, writing data, 
 *          and displaying memory content within defined address ranges.
 * @author  Bhavya Saravanan
 * @date    December 14, 2024
 * @version 1.0
 */



#include <at89c51ed2.h>
#include <stdio.h>
#include <stdint.h>
#include "code_memory.h"
#include "xram_memory.h"


#define DIVIDE_BY_16 (16)
#define NUMBER_BASE (16)
#define DATA_MAX (255)
#define ADDRESS_MAX (0x7FFF)


void xram_write(unsigned int address, unsigned char data);
unsigned char xram_read(unsigned int address);
void read_memory(void);
void memory_read(unsigned int start_address, unsigned int end_address);

void initialize_xram(void) {
    unsigned int address;
    for (address = 0x0000; address <= ADDRESS_MAX; address++) {
        xram_write(address, 0xFF);
    }
   
}

void xram_write(unsigned int address, unsigned char data) {
    unsigned char __xdata *ptr = (unsigned char __xdata *)address;
    *ptr = data;
}

unsigned char xram_read(unsigned int address) {
    unsigned char __xdata *ptr = (unsigned char __xdata *)address;
    return *ptr;
}

void write_memory(void) {
    unsigned int start_address, end_address;
    unsigned char data;

    printf("\r\n Enter Start Address to Write (Hex): ");
    //printf("\r\n ");
    start_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    printf("\r\n Enter End Address to Write (Hex): ");
    //printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);

    if (end_address < start_address) {
        printf("\r\n Error: End Address must be greater than or equal to Start Address.\r\n");
        return;
    }
    printf("\r\n");
    printf("\r\n Enter Data to Write (Hex): ");
     // printf("\r\n");
    data = parse_user_input(NUMBER_BASE);
    printf("\r\n");
    printf("\r\n---------------------------XRAM WRITE----------------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");

    for (unsigned int addr = start_address; addr <= end_address; addr++) {
        if ((addr - start_address) % DIVIDE_BY_16 == 0) {
            printf("\r\n%04X: ", addr);
        }
        printf("%02X  ", data);
        xram_write(addr, data);
    }
      printf("\r\n");

    printf("\r\n---------------------------------------------------------------------\r\n");
    printf("\r\n Data 0x%02X written to addresses 0x%04X to 0x%04X.\r\n", data, start_address, end_address);
}

void read_memory(void) {
    unsigned int start_address, end_address;

    printf("\r\n Enter Start Address to Read (Hex): ");
    //printf("\r\n");
    start_address = parse_user_input(NUMBER_BASE);
     printf("\r\n");
    printf("\r\n Enter End Address to Read (Hex): ");
    //printf("\r\n");
    end_address = parse_user_input(NUMBER_BASE);

    if (end_address < start_address) {
        printf("\r\n Error: End Address must be greater than or equal to Start Address.\r\n");
        return;
    }

    printf("\r\n--------------------------XRAM CONTENTS--------------------------\r\n");
    printf("\r\n");
    printf("Addr: +0  +1  +2  +3  +4  +5  +6  +7  +8  +9  +A  +B  +C  +D  +E  +F\r\n");
    memory_read(start_address, end_address);
    printf("\r\n------------------------------------------------------------------\r\n");
}

void memory_read(unsigned int start_address, unsigned int end_address) {
    unsigned char count = 0;

    while (start_address <= end_address) {
        if (count % DIVIDE_BY_16 == 0) {
            printf("\r\n%04X: ", start_address);
        }
        printf("%02X  ", xram_read(start_address));
        start_address++;
        count++;
    }
    printf("\r\n");
}