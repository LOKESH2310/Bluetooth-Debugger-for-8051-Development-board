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
 * @file    code_memory.h
 * @brief   Header file for code memory operations.
 * @details Declares functions for reading and processing code memory data.
 * @author  Bhavya Saravanan
 * @date    December 14, 2024
 * @version 1.0
 */



#ifndef _code_memory_H_
#define _code_memorT_H_

/**
 * @brief   Reads and displays code memory content within a specified range.
 * @param   None
 * @return  None
 */
void read_code_memory(void);

/**
 * @brief   Reads code memory data from the specified start to end address.
 * @param   start_address - The starting address in code memory.
 * @param   end_address - The ending address in code memory.
 * @return  None
 */
void code_memory_read(unsigned int start_address, unsigned int end_address);

/**
 * @brief   Parses user input as an unsigned integer based on the provided base.
 * @param   base - The numerical base (e.g., 16 for hexadecimal).
 * @return  The parsed integer value.
 */
unsigned int parse_user_input(unsigned char base);

/**
 * @brief   Prints a hexadecimal representation of a number with a specified width.
 * @param   num - The number to be printed.
 * @param   width - The number of hexadecimal digits to display.
 * @return  None
 */
void print_hex_number(uint32_t num, unsigned char width);

/**
 * @brief   Converts an integer value to its corresponding hexadecimal character.
 * @param   num - The integer value (0-15) to convert.
 * @return  The hexadecimal character ('0'-'9', 'A'-'F').
 */
char int_to_char(int num);

/**
 * @brief   Converts a hexadecimal character to its corresponding integer value.
 * @param   ch - The hexadecimal character ('0'-'9', 'A'-'F', 'a'-'f').
 * @return  The integer value (0-15).
 */
unsigned char char_to_int(unsigned char ch);

#endif 
