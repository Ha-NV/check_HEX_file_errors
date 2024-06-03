/**
 * @file intel_hex_file_analyzer.c
 * @brief This file contains the definitions of the Intel Hex file analyzer functions.
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
#include "intel_hex_file_analyzer.h"             /* Include header file of this function file */
#include "record_handler.h"  /* Include header file of lower layer */

/*******************************************************************************
 * Code
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
int32_t analyzeIntelHexFile(FILE *fptr, Error_t *error)
{
    int8_t line[100];               /* Initialize a character array to store each line of the file */
    int8_t continue_process = 1;    /* Initialize a flag to control the loop */
    int32_t line_of_file = 1;       /* Initialize a counter to keep track of the line number */

    /* Loop through each line of the file until the end of the file is reached or an error is encountered */
    while ((fgets(line, sizeof(line), fptr) != NULL) && (continue_process))
    {
        /* Check the validity of the current line and store the error code in the error structure */
        error->error_code = checkRecord(line);
        /* If an error is encountered, set the continue_process flag to 0 and
        store the line number in the error structure */
        if (error->error_code != 0)
        {
            continue_process = 0;
            error->error_line = line_of_file;
        }
        /* If no error is encountered, increment the line number */
        else
        {
            line_of_file += 1;
        }
    }
    /* Return the error code stored in the error structure */
    return error->error_code;
}

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
int32_t checkEOF(FILE *fptr, Error_t *error)
{
    int8_t line[100];               /* Initialize a character array to store each line of the file */
    int8_t EOF_line[100];           /* Initialize a character array to store the End-Of-File record */
    int8_t last_line[100];          /* Initialize a character array to store the last line of the file */
    int8_t found_EOF = 0;           /* Initialize a flag to indicate if the End-Of-File record is found */
    int32_t line_of_file = 1;       /* Initialize a counter to keep track of the line number */

    /* Loop through each line of the file until the end of the file is reached */
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        /* Copy the current line to the last line */
        strcpy(last_line, line);
        /* Check if the current line is the End-Of-File record */
        if (strncmp(line, ":00000001FF", 11) == 0)
        {
            /* If the End-Of-File record is found for the first time, store the line number in the error structure */
            if (!found_EOF)
            {
                error->error_line = line_of_file;
            }
            else
            {
                /* Do nothing */
            }
            /* Increment the count of the End-Of-File record */
            found_EOF += 1;
            /* Copy the End-Of-File record to the EOF_line */
            strcpy(EOF_line, line);
        }
        else
        {
            /* Do nothing */
        }
        line_of_file += 1;
    }
    /* If the End-Of-File record is not found, set the error code to 1 */
    if (!found_EOF)
    {
        error->error_code = 1;
    }
    /* If the End-Of-File record is found only once, check if the last line is the same as the End-Of-File record */
    else if (found_EOF == 1)
    {
        /* If last line of file is different from End-Of-File record, set the error code to 2 */
        if (strcmp(last_line, EOF_line) != 0)
        {
            error->error_code = 2;
        }
        /* If not then set the error code to 0 */
        else
        {
            error->error_code = 0;
        }
    }
    /* If the End-Of-File record is found more than once, set the error code to 3 */
    else
    {
        error->error_code = 3;
    }
    /* Return the error code */
    return error->error_code;
}

/**
 * @brief This function prints the entire content of an Intel Hex File.
 *
 * The function reads each line of the file and prints it along with its line number.
 * It also calls the displayRecordInfo function to print the information of each records.
 *
 * @param fptr The file pointer to the Intel Hex File.
 */
void printIntelHexFile(FILE *fptr)
{
    int8_t line[100];               /* Initialize a character array to store each line of the file */
    int32_t line_of_file = 1;       /* Initialize a variable to store the line number */

    /* Read each line of the file until the end of the file is reached */
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        printf("----------------\n");
        /* Print the line number */
        printf("Line %d of file: \n", line_of_file);
        /* Print the line of file */
        printf("%s\n", line);
        /* Display the information of the record */
        displayRecordInfo(line, line_of_file);
        printf("----------------\n\n");
        line_of_file += 1;
    }
} /* EOF */

