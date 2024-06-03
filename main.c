/**
 * @file main.c
 * @brief This file contains the main function of the application layer.
 *
 * The main function is responsible for analyzing the Intel Hex file and checking the validity of the file.
 * If the Intel Hex file is not valid, it returns an error code.
 * The program will handle any errors that occur during the analysis of the file.
 * If an error is encountered, it prints an error message and the line number where the error occurred.
 *
 * @author Viet Ha Nguyen
 * @date 4/26/2024
 */

/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>      /* Include standard input and output library for printf, scanf, ... */
#include <stdint.h>     /* Include standard integer types library for fixed-width integers like int8_t, uint64_t, etc. */
#include <string.h>     /* For NULL character */
#include "intel_hex_file_analyzer.h"   /* Include header file of lower layer */

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief The main function of the application.
 *
 * The function is responsible for analyzing the Intel Hex file and checking the validity of the file.
 * If the Intel Hex file is not valid, it returns an error code.
 * The program will handle any errors that occur during the analysis of the file.
 * If an error is encountered, it prints an error message and the line number where the error occurred.
 *
 * @return An integer indicating the exit status of the program.
 */
int32_t main(void)
{
    int8_t line[100];            /* Initialize a character array to store each line of the file */
    int8_t correct_format = 0;   /* Initialize a flag to indicate if the file has correct format */
    int32_t file_error_code = 0; /* Initialize a variable to store the error code of the file */
    int32_t EOF_error_code = 0;  /* Initialize a variable to store the error code of the End-Of-File record */

    Error_t file_problem;        /* Initialize an error structure to store the error code and line number of the file */
    Error_t EOF_problem;         /* Initialize an error structure to store the error code and line number of the End-Of-File record */

    /* Open the Intel Hex file in read mode */
    FILE *fptr = fopen("hex_file.hex", "r");

    /* Check if the file is successfully opened */
    if (fptr == NULL)
    {
        printf("Error: Can not open file.\n");
    }
    else
    {
        /* Analyze the Intel Hex file and store the error code, the line number where the error occurred
        in the file_problem */
        file_error_code = analyzeIntelHexFile(fptr, &file_problem);
        /* Check the error code of the file */
        switch (file_error_code)
        {
            case 1:
            {
                printf("Error at line %d: There is no ':' character at the beginning of the line.\n", file_problem.error_line);
                break;
            }
            case 2:
            {
                printf("Error at line %d: Record format isn't valid.\n", file_problem.error_line);
                break;
            }
            case 3:
            {
                printf("Error at line %d: Record type isn't valid.\n", file_problem.error_line);
                break;
            }
            case 4:
            {
                printf("Error at line %d: The number of bytes of data field and record-length field aren't the same.\n", file_problem.error_line);
                break;
            }
            case 5:
            {
                printf("Error at line %d: Checksum field doesn't match the actual calculation.\n", file_problem.error_line);
                break;
            }
            /* If the error code is not 1, 2, 3, 4, or 5, set the correct_format flag to 1 */
            default:
            {
                correct_format = 1;
            }
        }
        /* Close the file */
        fclose(fptr);

        /* If the file has correct format, open the file again and check the End-Of-File record */
        if (correct_format)
        {
            /* Open the Intel Hex file in read mode */
            fptr = fopen("hex_file.hex", "r");
            /* Check the End-Of-File record and store the error code, the line number where the error occurred
            in the EOF_problem */
            EOF_error_code = checkEOF(fptr, &EOF_problem);
            /* Check the error code of the End-Of-File record */
            switch (EOF_error_code)
            {
                case 1:
                {
                    printf("File error: File is missing End-Of-File record!!!\n");
                    break;
                }
                case 2:
                {
                    printf("Error at line %d: End-Of-File record must at the end of file!!!\n", EOF_problem.error_line);
                    break;
                }
                case 3:
                {
                    printf("Error at line %d: File mustn't have more than one End-Of-File record!!!\n", EOF_problem.error_line);
                    break;
                }
                /* If the error code is not 1, 2, or 3, print a success message */
                default:
                {
                    printf("\n--> INTEL-HEX FILE HAS CORRECT FORMAT, WITHOUT ANY ERRORS.\n\n");
                    printf("--> BELOW IS THE INFORMATION OF ALL FILE'S RECORDS . . .\n\n");
                }
            }
            /* Close the file */
            fclose(fptr);
        }
        else
        {
            /* Do nothing */
        }
    }

    /* If the file has correct format and the End-Of-File record is valid, open the file again and
    print the entire content (information of each lines as well as records) of the file */
    if (correct_format && !EOF_error_code)
    {
        /* Open the Intel Hex file in read mode */
        fptr = fopen("hex_file.hex", "r");
        /* Print the entire content of the file */
        printIntelHexFile(fptr);
        /* Close the file */
        fclose(fptr);
    }
    /* If the file doesn't have correct format or the End-Of-File record is not valid, print an error message */
    else
    {
        printf("\n--> STOP CHECKING THE FILE BECAUSE FILE'S FORMAT IS NOT VALID . . . \n");
    }
    /* Return 0 to indicate that the program has finished */
    return 0;
} /* EOF */


