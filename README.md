# BÀI TẬP: THUẬT TOÁN DES VÀ AES-128

## 1. Giới thiệu

Dự án thực hiện cài đặt và kiểm thử hai thuật toán mã hóa đối xứng DES (Data Encryption Standard) và AES-128 (Advanced Encryption Standard) bằng ngôn ngữ lập trình C++.

Mục tiêu của dự án là tìm hiểu nguyên lý hoạt động, quá trình mã hóa, giải mã và kiểm tra tính chính xác của từng thuật toán thông qua các bộ dữ liệu kiểm thử (Test Vector).

## 2. Cấu trúc dự án

- `main.cpp`: Chứa mã nguồn cài đặt thuật toán DES và AES-128.
- `README.md`: Giới thiệu dự án và hướng dẫn sử dụng.
- `results.txt`: Lưu kết quả thực thi và kiểm định thuật toán.
- `test_vectors.txt`: Lưu các bộ dữ liệu kiểm thử.

## 3. Thuật toán DES

Chương trình thực hiện các bước chính của thuật toán DES:

- Hoán vị ban đầu (Initial Permutation - IP).
- Sinh 16 khóa con cho 16 vòng mã hóa.
- Thực hiện 16 vòng biến đổi Feistel.
- Hoán vị cuối (Final Permutation - FP).
- Thực hiện mã hóa và giải mã để kiểm tra kết quả.

Chương trình hiển thị các giá trị trung gian trong từng vòng để phục vụ việc kiểm định.

## 4. Thuật toán AES-128

Chương trình cài đặt thuật toán AES với độ dài khóa 128 bit, bao gồm:

- Mở rộng khóa (Key Expansion).
- Phép biến đổi AddRoundKey.
- Phép biến đổi SubBytes.
- Phép biến đổi ShiftRows.
- Phép biến đổi MixColumns.
- Thực hiện đầy đủ 10 vòng mã hóa.

Các trạng thái trung gian sau từng phép biến đổi được hiển thị để kiểm tra quá trình thực hiện thuật toán.

## 5. Dữ liệu kiểm thử (Test Vector)

### DES

- Khóa: `133457799BBCDFF1`
- Bản rõ: `0123456789ABCDEF`
- Bản mã mong đợi: `85E813540F0AB405`

### AES-128

- Khóa: `000102030405060708090A0B0C0D0E0F`
- Bản rõ: `00112233445566778899AABBCCDDEEFF`
- Bản mã mong đợi: `69C4E0D86A7B0430D8CDB78070B4C55A`

## 6. Hướng dẫn biên dịch và chạy chương trình

Yêu cầu: Máy tính đã cài đặt trình biên dịch G++.

Biên dịch chương trình:

```bash
g++ -std=c++11 -O2 -Wall -Wextra main.cpp -o crypto
```

Chạy chương trình trên Windows:

```bash
.\crypto.exe
```

## 7. Kiểm định kết quả

Chương trình thực hiện các nội dung kiểm tra:

- So sánh bản mã thực tế với bản mã mong đợi.
- Kiểm tra kết quả giải mã có khôi phục đúng bản rõ ban đầu hay không.
- Hiển thị các trạng thái trung gian trong quá trình thực hiện DES và AES-128.
- Thông báo kết quả kiểm định PASS hoặc FAIL.

## 8. Kết luận

Dự án đã triển khai hai thuật toán mã hóa đối xứng DES và AES-128 bằng C++. Thông qua việc hiển thị các trạng thái trung gian và kiểm tra bằng Test Vector, chương trình hỗ trợ đánh giá tính chính xác của quá trình mã hóa và giải mã.