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
 * @file    xram_memory.h
 * @brief   Header file for XRAM memory operations.
 * @details Declares functions for reading, writing, and managing XRAM memory data.
 *          Includes initialization and memory content display features.
 * @date    December 14, 2024
 * @version 1.0
 */



#ifndef _xram_memory_H_
#define _xram_memorT_H_
/**
 * @brief   Initializes the XRAM memory space with default values.
 * @param   None
 * @return  None
 */
void initialize_xram(void);

/**
 * @brief   Writes data to a specified XRAM memory address.
 * @param   address - The XRAM memory address to write to.
 * @param   data - The data value to write.
 * @return  None
 */
void xram_write(unsigned int address, unsigned char data);

/**
 * @brief   Reads data from a specified XRAM memory address.
 * @param   address - The XRAM memory address to read from.
 * @return  The data value read from the address.
 */
unsigned char xram_read(unsigned int address);

/**
 * @brief   Displays the content of a specified XRAM memory range.
 * @param   None
 * @return  None
 */
void read_memory(void);

/**
 * @brief   Reads and displays XRAM memory data within a specified range.
 * @param   start_address - The starting address in XRAM memory.
 * @param   end_address - The ending address in XRAM memory.
 * @return  None
 */
void memory_read(unsigned int start_address, unsigned int end_address);

/**
 * @brief   Writes a user-defined value to a specified range in XRAM memory.
 * @param   None
 * @return  None
 */
void write_memory(void);

#endif 
