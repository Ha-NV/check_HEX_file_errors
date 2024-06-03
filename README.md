# check_HEX_file_errors
    Viết chương trình đọc và kiểm tra lỗi các trường trong file Intel hex
    (https://en.wikipedia.org/wiki/Intel_HEX)
    
    - Tách và phân tích các filed của record. (Start code Byte count Address Record type Data Checksum)
    
    - Nếu các trường Byte count, Checksum, bị sai so với thực tế hoặc cấu trúc record không đúng (thiếu start code) thì record đó bị lỗi.
    
    - Nếu file tồn tại 1 record lỗi thì dừng kiểm tra lỗi và thông báo: File đã bị lỗi do tồn tại 1 record lỗi.
    
    - Nếu không có End Of File record ở cuối file, cần đưa ra thông báo: File bị lỗi không tồn tại End Of File record.
    
    - Nếu file đúng theo format, không bị lỗi, hiển thị nội dung Absolute memory address và Data field của các record theo thứ tự record đọc vào.
