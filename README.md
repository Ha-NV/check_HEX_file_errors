# check_HEX_file_errors
    Write a program to read and check fields in Intel hex file for errors
    (https://en.wikipedia.org/wiki/Intel_HEX)
    
    - Separate and analyze fields of records. (Start code Byte count Address Record type Data Checksum)
    
    - If the fields Byte count, Checksum, are wrong compared to reality or the record structure is 
    incorrect (missing start code), then that record is error.
    
    - If the file contains an error record, stop checking for errors and notify: The file is corrupted 
    due to the existence of an error record.
    
    - If there is no End Of File record at the end of the file, a message should be given: The file is
    corrupted and does not have an End Of File record.
    
    - If the file is in the correct format, without errors, display the Absolute memory address and 
    Data fields of the records in the order the records are read.
