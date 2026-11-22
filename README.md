# DES va AES-128 (C++)

Bai tap cai dat thuat toan DES va AES-128, co in cac trang thai trung gian de kiem dinh.

## Yeu cau
- Trinh bien dich C++ ho tro C++11 tro len.
- Khong can cai thu vien ngoai.

## Cach bien dich va chay

Mo Terminal tai thu muc du an va chay:

```bash
g++ -std=c++11 -O2 -Wall -Wextra main.cpp -o crypto
```

Windows:
```bash
crypto.exe
```

macOS/Linux:
```bash
./crypto
```

## Test vector dang dung

### DES
- Key: `133457799BBCDFF1`
- Plaintext: `0123456789ABCDEF`
- Ciphertext mong doi: `85E813540F0AB405`

### AES-128
- Key: `000102030405060708090A0B0C0D0E0F`
- Plaintext: `00112233445566778899AABBCCDDEEFF`
- Ciphertext mong doi: `69C4E0D86A7B0430D8CDB78070B4C55A`

Chuong trinh in khoa vong DES, cac trang thai L/R cua 16 vong DES va cac buoc SubBytes, ShiftRows, MixColumns, AddRoundKey cua AES-128. Cuoi cung, chuong trinh doi chieu ciphertext voi ket qua mong doi va kiem tra giai ma.

**Luu y:** Day la cac test vector chuan de kiem tra ban dau. Neu giang vien da cung cap test vector khac tren lop, can bo sung/dung dung bo du lieu do trong bai nop.
