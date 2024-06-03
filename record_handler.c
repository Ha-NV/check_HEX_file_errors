/**
 * @file record_handler.c
 * @brief This file contains the implementation of the record handler functions.
 *
 * The record handler is responsible for processing the Intel Hex records.
 * It provides function checkRecord to check the validity of the record and returns error code to upper layer (HAL layer),
 * extracts the data, and stores it in the appropriate format.
 * The record handler also provides function displayRecordInfo to print the contents of a record.
 *
 * @author Viet Ha Nguyen
 * @date 4/26/2024
 */
/*******************************************************************************
 * Include
 ******************************************************************************/
#include "record_handler.h"      /* Include header file of this function file */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief This function prints the fields of a record.
 *
 * The function prints the record-length field, address field, HEX record type, data field,
 * and checksum field of the record.
 *
 * @param record The record structure to be printed.
 */
static void printRecord(IntelHexRecord_t record);

/*******************************************************************************
 * Code
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
int32_t checkRecord(int8_t line[])
{
    int8_t byte_count_str[3];           /* The byte count in the record as a string */
    int8_t address_str[5];              /* The address in the record as a string */
    int8_t record_type_str[3];          /* The record type in the record as a string */
    int32_t i = 0;                      /* Loop counter */
    int32_t error_code = 0;             /* Error code, initialized to 0 */
    int32_t data_byte = 0;              /* The number of data bytes in the record */
    int32_t checksum_record = 0;        /* The checksum in the record */
    int32_t calculated_checksum = 0;    /* The calculated checksum in reality based on other fields */

    IntelHexRecord_t record;            /* Declaring an Intel Hex record */

    /* Check if the record starts with a colon */
    if (line[0] != ':')
    {
        /* If not then set error code to 1 */
        error_code = 1;
    }
    /* Parse the byte count, address, and record type from the record */
    else if (sscanf(line, ":%2s%4s%2s", byte_count_str, address_str, record_type_str) != 3)
    {
        /* If not then set error code to 2 */
        error_code = 2;
    }
    /* Check if the record type is valid */
    else if ((strcmp(record_type_str, "00") != 0) && (strcmp(record_type_str, "01") != 0) && (strcmp(record_type_str, "02") != 0) &&
             (strcmp(record_type_str, "04") != 0) && (strcmp(record_type_str, "05") != 0))
    {
        /* If not then set error code to 3 */
        error_code = 3;
    }
    else
    {
        /* Convert the byte count from a string to a hexidecimal integer */
        sscanf(byte_count_str, "%X", &(record.byte_count));
        /* Convert the address from a string to a hexidecimal integer */
        sscanf(address_str, "%X", &record.address);
        /* Convert the record type from a string to a hexidecimal integer */
        sscanf(record_type_str, "%X", &(record.record_type));

        /* Calculate the number of data bytes in the record */
        data_byte = (strlen(line) - 11 ) / 2;

        /* Check if the number of data bytes in the record matches the byte count */
        if (data_byte != record.byte_count)
        {
            /* If not then set error code to 4 */
            error_code = 4;
        }
        else
        {
            /* Allocate memory for the record.data */
            record.data = (uint8_t *)malloc(record.byte_count * sizeof(uint8_t));
            /* Parse the data from the record */
            for (i = 0; i < record.byte_count; i++)
            {
                sscanf(line + 9 + (i * 2), "%02X", &(record.data[i]));
            }

            /* Parse the checksum from the record */
            sscanf(line + strlen(line) - 3, "%02X", &checksum_record);

            /* Calculate checksum in reality based on other record's fields */
            calculated_checksum = record.byte_count + (record.address >> 8) + (record.address & 0xFF) + record.record_type;
            /* Add the data to the checksum */
            for (i = 0; i < record.byte_count; i++)
            {
                calculated_checksum += record.data[i];
            }
            /* Take the two's complement of the checksum */
            calculated_checksum = (~calculated_checksum + 1) & 0xFF;
            /* Check if the calculated checksum matches the checksum in the record */
            if (calculated_checksum != checksum_record)
            {
                /* If not then set error code to 5 */
                error_code = 5;
            }
            else
            {
                /* Set the checksum in the record */
                record.checksum = checksum_record;
            }
            free(record.data);
        }
    }
    /* Return the error code */
    return error_code;
}

/**
 * @brief This function displays the entire information of the record.
 *
 * The function parses the line to extract the byte count, address, record type, data, and checksum of the record.
 * It then checks the record type and prints the appropriate absolute memory address.
 *
 * @param line The line that contain the record.
 * @param record_number The number of the record of Intel HEX file.
 */
void displayRecordInfo(int8_t line[], int32_t record_number)
{
    int32_t i = 0;                      /* Initialize the counter variable */
    int32_t abs_address = 0;            /* Initialize the absolute address variable */
    static int32_t base_address = 0;    /* Initialize the base address variable */

    IntelHexRecord_t record;            /* Declaring an Intel Hex record */

    /* Allocate memory for the data in the record */
    record.data = (uint8_t *)malloc(record.byte_count * sizeof(uint8_t));

    /* Parse the byte count, address, and record type from the line */
    sscanf(line, ":%02X%04X%02X", &(record.byte_count), &(record.address), &(record.record_type));
    /* Parse the checksum from the line */
    sscanf(line + strlen(line) - 3, "%02X", &(record.checksum));
    /* Parse the data from the line */
    for (i = 0; i < record.byte_count; i++)
    {
        sscanf(line + 9 + (i * 2), "%02X", &(record.data[i]));
    }
    /* Check if the record type is a data record */
    if (record.record_type == 0x00)
    {
        /* Print the record number and type */
        printf("*** INFORMATION OF RECORD %d: DATA RECORD ***\n\n", record_number);
        /* Print the details of the record's fields */
        printRecord(record);
        /* Set the base address to the address in the record */
        base_address = record.address;
    }
    /* Check if the record type is an extended segment address record */
    else if (record.record_type == 0x02)
    {
        /* Print the record number and type */
        printf("*** INFORMATION OF RECORD %d: EXTENDED SEGMENT ADDRESS RECORD ***\n\n", record_number);
        /* Print the details of the record's fields */
        printRecord(record);
        /* Calculate the absolute address */
        abs_address = base_address + (record.data[0] * 0x1000) + (record.data[1] * 0x10);
        /* Print the address from the data record's address field */
        printf("-> Address from the data record's address field: %04X\n", base_address);
        /* Print the absolute memory address */
        printf("-> Absolute memory address: %08X\n", abs_address);
    }
    /* Check if the record type is an extended linear address record */
    else if (record.record_type == 0x04)
    {
        /* Print the record number and type */
        printf("*** INFORMATION OF RECORD %d: EXTENDED LINEAR ADDRESS RECORD ***\n\n", record_number);
        /* Print the details of the record's fields */
        printRecord(record);
        /* Calculate the absolute address */
        abs_address = base_address + (record.data[0] * 0x1000000) + (record.data[1] * 0x10000);
        /* Print the address from the data record's address field */
        printf("-> Address from the data record's address field: %04X\n", base_address);
        /* Print the absolute memory address */
        printf("-> Absolute memory address: %08X\n", abs_address);
    }
    /* If record is an EOF */
    else
    {
        /* Print the record number and type */
        printf("*** INFORMATION OF RECORD %d: END-OF-FILE RECORD ***\n\n", record_number);
    }
    free(record.data);
}

/**
 * @brief This function prints the fields of a record.
 *
 * The function prints the record-length field, address field, HEX record type, data field,
 * and checksum field of the record.
 *
 * @param record The record structure to be printed.
 */
static void printRecord(IntelHexRecord_t record)
{
    int32_t i = 0;          /* Initialize the counter variable */

    /* Print the record-length field */
    printf("Record-length field: %02X <=> %d bytes of data\n", record.byte_count, record.byte_count);
    /* Print the address field */
    printf("Address field: %04X\n", record.address);
    /* Print the HEX record type */
    printf("HEX record type: %02X\n", record.record_type);
    /* Print the data field */
    printf("Data field: ");
    /* Print the data in the data field */
    for (i = 0; i < record.byte_count; i++)
    {
        printf("%02X", record.data[i]);
    }
    /* Print the checksum field */
    printf("\nChecksum field: %02X\n", record.checksum);
} /* EOF */

