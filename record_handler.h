/**
 * @file record_handler.h
 * @brief This file contains the prototypes of the record handler functions.
 *
 * The record handler is responsible for processing the Intel Hex records.
 * It provides function checkRecord to check the validity of the record and returns error code
 * to upper layer (HAL layer), extracts the data, and stores it in the appropriate format.
 * The record handler also provides function displayRecordInfo to print the contents of a record.
 *
 * @author Viet Ha Nguyen
 * @date 4/26/2024
 */
#include <stdint.h>   /* Include standard integer types library for fixed-width integers like int8_t, uint64_t, etc. */
#include <stdio.h>    /* Include standard input and output library for printf, scanf, ... */
#include <stdlib.h>   /* For malloc(), free() functions*/
#include <string.h>   /* For strcpy(), strcmp() functions*/

/*******************************************************************************
 * Header guards
 ******************************************************************************/
#ifndef RECORD_HANDLER_H
#define RECORD_HANDLER_H

/*******************************************************************************
 * Declarations
 ******************************************************************************/
/**
 * @brief Structure to hold the information of an Intel Hex Record.
 *
 * The structure contains the byte count, address, record type, data, and checksum of the record.
 */
typedef struct
{
    uint32_t byte_count;
    uint32_t address;
    uint32_t record_type;
    uint8_t *data;
    uint32_t checksum;
} IntelHexRecord_t;

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/**
 * @brief This function checks the validity of the Intel Hex record.
 *
 * The function first checks if the record starts with a colon.
 * It then checks if the byte count, address, and record type has correct syntax.
 * The function also checks if the record type is valid (00, 01, 02, 04, or 05).
 * If all checks pass, the function calculates the checksum of the record and compares it
 * with the checksum in the record.
 * If the checksums match, the function returns 0, indicating that the record syntax is valid.
 * If any of the checks fail, the function returns an error code that indicate the type of error.
 *
 * @param line An Intel-Hex-File's line that contain the record's information.
 * @return An error code indicating the result of the check.
 */
int32_t checkRecord(int8_t line[]);

/**
 * @brief This function displays the entire information of the record.
 *
 * The function parses the line to extract the byte count, address, record type, data, and checksum of the record.
 * It then checks the record type and prints the appropriate absolute memory address.
 *
 * @param line The line that contain the record.
 * @param record_number The number of the record of Intel HEX file.
 */
void displayRecordInfo(int8_t line[], int32_t record_number);

#endif /* RECORD_HANDLER_H */

