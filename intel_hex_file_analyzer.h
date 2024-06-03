/**
 * @file intel_hex_file_analyzer.h
 * @brief This file contains the prototypes of the Intel Hex file analyzer functions.
 *
 * The Intel Hex file analyzer is responsible for analyzing the Intel Hex file.
 * It provides function analyzeIntelHexFile to check the validity of the file and returns error code
 * to upper layer (Application layer).
 * The Intel Hex file analyzer also provides function checkEOF to check if the End-Of-File record valid.
 *
 * @author Viet Ha Nguyen
 * @date 4/26/2024
 */
/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdint.h>   /* Include standard integer types library for fixed-width integers like int8_t, uint64_t, etc. */
#include <stdio.h>    /* Include standard input and output library for printf, scanf, ... */
#include <stdlib.h>   /* For malloc(), free() functions*/
#include <string.h>   /* For strcpy(), strcmp() functions*/

/*******************************************************************************
 * Header guards
 ******************************************************************************/
#ifndef INTEL_HEX_FILE_ANALYZER_H
#define INTEL_HEX_FILE_ANALYZER_H

/*******************************************************************************
 * Declarations
 ******************************************************************************/
typedef struct
{
    uint32_t error_code;
    uint32_t error_line;
} Error_t;

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/**
 * @brief This function analyzes the Intel Hex file.
 *
 * The function reads the Intel Hex file line by line and checks the validity of each line by using
 * checkRecord function of lower layer (Middleware).
 * If any line is found to be invalid, the function stops analyzing and returns the error code.
 * If all lines are valid, the function returns 0, indicating that the file is valid.
 *
 * @param fptr The file pointer to the Intel Hex file.
 * @param error The error structure to store the error code and line number.
 * @return An error code indicating the result of the analysis.
 */
int32_t analyzeIntelHexFile(FILE *fptr, Error_t *error);

/**
 * @brief This function checks the End-Of-File record.
 *
 * The function reads the file line by line and checks if the line is the End-Of-File record.
 * If the End-Of-File record is found and this record is at the end of file, the function returns 0,
 * indicating that the file is valid.
 * If the End-Of-File record is not found or End-Of-File record is found but it's not at the end of file,
 * the function returns an error code.
 *
 * @param fptr The file pointer to the Intel Hex file.
 * @param error The error structure to store the error code and line number.
 * @return An error code indicating the result of the check.
 */
int32_t checkEOF(FILE *fptr, Error_t *error);

/**
 * @brief This function prints the entire content of an Intel Hex File.
 *
 * The function reads each line of the file and prints it along with its line number.
 * It also calls the displayRecordInfo function to print the information of each records.
 *
 * @param fptr The file pointer to the Intel Hex File.
 */
void printIntelHexFile(FILE *fptr);

#endif /* INTEL_HEX_FILE_ANALYZER_H */

