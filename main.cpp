
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

// ============================== TIEN ICH ==============================
string hex64(uint64_t x, int digits = 16) {
    stringstream ss;
    ss << uppercase << hex << setw(digits) << setfill('0') << x;
    return ss.str();
}
string hexBytes(const array<uint8_t,16>& a) {
    string s;
    for (uint8_t b : a) {
        stringstream ss; ss << uppercase << hex << setw(2) << setfill('0') << (int)b;
        s += ss.str();
    }
    return s;
}
uint64_t parseHex64(const string& s) {
    return stoull(s, nullptr, 16);
}
array<uint8_t,16> parseHex128(const string& s) {
    array<uint8_t,16> a{};
    for (int i=0;i<16;i++) a[i] = (uint8_t)stoul(s.substr(2*i,2), nullptr, 16);
    return a;
}

// ================================ DES ==================================
const int IP[64] = {
58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
const int FP[64] = {
40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
const int E[48] = {
32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,
12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,
24,25,26,27,28,29,28,29,30,31,32,1};
const int P[32] = {
16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
const int PC1[56] = {
57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,
59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,
31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,
29,21,13,5,28,20,12,4};
const int PC2[48] = {
14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,
26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,
51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
const int SHIFTS[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
const int SBOX[8][4][16] = {
{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};
uint64_t permute(uint64_t in, const int* table, int outBits, int inBits) {
    uint64_t out=0;
    for(int i=0;i<outBits;i++) out=(out<<1)|((in>>(inBits-table[i]))&1ULL);
    return out;
}
vector<uint64_t> desKeys(uint64_t key, bool show=false) {
    uint64_t k=permute(key,PC1,56,64);
    uint32_t c=(uint32_t)((k>>28)&0x0FFFFFFF), d=(uint32_t)(k&0x0FFFFFFF);
    vector<uint64_t> keys;
    for(int i=0;i<16;i++){
        c=((c<<SHIFTS[i])|(c>>(28-SHIFTS[i])))&0x0FFFFFFF;
        d=((d<<SHIFTS[i])|(d>>(28-SHIFTS[i])))&0x0FFFFFFF;
        uint64_t cd=((uint64_t)c<<28)|d;
        uint64_t sub=permute(cd,PC2,48,56);
        keys.push_back(sub);
        if(show) cout<<"K"<<setw(2)<<setfill('0')<<i+1<<" = "<<hex64(sub,12)<<setfill(' ')<<"\n";
    }
    return keys;
}
uint32_t desF(uint32_t r, uint64_t key) {
    uint64_t x=permute(r,E,48,32)^key;
    uint32_t sOut=0;
    for(int i=0;i<8;i++){
        int six=(x>>(42-6*i))&0x3F;
        int row=((six&0x20)>>4)|(six&1);
        int col=(six>>1)&0xF;
        sOut=(sOut<<4)|SBOX[i][row][col];
    }
    return (uint32_t)permute(sOut,P,32,32);
}
uint64_t desBlock(uint64_t block, const vector<uint64_t>& keys, bool trace=false) {
    uint64_t x=permute(block,IP,64,64);
    uint32_t l=(uint32_t)(x>>32), r=(uint32_t)x;
    if(trace) cout<<"Sau IP: L0="<<hex64(l,8)<<" R0="<<hex64(r,8)<<"\n";
    for(int i=0;i<16;i++){
        uint32_t oldR=r;
        uint32_t f=desF(r,keys[i]);
        r=l^f; l=oldR;
        if(trace) cout<<"Vong "<<setw(2)<<setfill('0')<<i+1<<setfill(' ')
                      <<": K="<<hex64(keys[i],12)<<" L="<<hex64(l,8)
                      <<" R="<<hex64(r,8)<<"\n";
    }
    uint64_t pre=((uint64_t)r<<32)|l;
    uint64_t out=permute(pre,FP,64,64);
    if(trace) cout<<"Truoc FP (R16||L16): "<<hex64(pre)<<"\n";
    return out;
}

// ============================== AES-128 ================================
const uint8_t SBOX_AES[256] = {
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};
const uint8_t INV_SBOX_AES[256] = {
0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};
const uint8_t RCON[11] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};
using Block = array<uint8_t,16>;
using RoundKeys = array<uint8_t,176>;

void printState(const string& label, const Block& s) {
    cout << label << " = " << hexBytes(s) << "\n";
}
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p=0;
    for(int i=0;i<8;i++){
        if(b&1) p^=a;
        bool hi=a&0x80; a<<=1; if(hi) a^=0x1B;
        b>>=1;
    }
    return p;
}
RoundKeys expandKey(const Block& key) {
    RoundKeys w{};
    copy(key.begin(),key.end(),w.begin());
    int bytes=16, rcon=1;
    uint8_t t[4];
    while(bytes<176){
        for(int i=0;i<4;i++) t[i]=w[bytes-4+i];
        if(bytes%16==0){
            uint8_t tmp=t[0]; t[0]=t[1]; t[1]=t[2]; t[2]=t[3]; t[3]=tmp;
            for(int i=0;i<4;i++) t[i]=SBOX_AES[t[i]];
            t[0]^=RCON[rcon++];
        }
        for(int i=0;i<4;i++){ w[bytes]=w[bytes-16]^t[i]; bytes++; }
    }
    return w;
}
void addRoundKey(Block& s, const RoundKeys& w, int round) {
    for(int i=0;i<16;i++) s[i]^=w[round*16+i];
}
void subBytes(Block& s, const uint8_t* box) { for(auto& b:s) b=box[b]; }
void shiftRows(Block& s) {
    Block t=s;
    for(int r=0;r<4;r++) for(int c=0;c<4;c++) s[r+4*c]=t[r+4*((c+r)%4)];
}
void invShiftRows(Block& s) {
    Block t=s;
    for(int r=0;r<4;r++) for(int c=0;c<4;c++) s[r+4*c]=t[r+4*((c-r+4)%4)];
}
void mixColumns(Block& s) {
    for(int c=0;c<4;c++){
        int i=4*c; uint8_t a=s[i],b=s[i+1],d=s[i+2],e=s[i+3];
        s[i]=gmul(a,2)^gmul(b,3)^d^e;
        s[i+1]=a^gmul(b,2)^gmul(d,3)^e;
        s[i+2]=a^b^gmul(d,2)^gmul(e,3);
        s[i+3]=gmul(a,3)^b^d^gmul(e,2);
    }
}
void invMixColumns(Block& s) {
    for(int c=0;c<4;c++){
        int i=4*c; uint8_t a=s[i],b=s[i+1],d=s[i+2],e=s[i+3];
        s[i]=gmul(a,14)^gmul(b,11)^gmul(d,13)^gmul(e,9);
        s[i+1]=gmul(a,9)^gmul(b,14)^gmul(d,11)^gmul(e,13);
        s[i+2]=gmul(a,13)^gmul(b,9)^gmul(d,14)^gmul(e,11);
        s[i+3]=gmul(a,11)^gmul(b,13)^gmul(d,9)^gmul(e,14);
    }
}
Block aesEncrypt(Block state, const Block& key, bool trace=false) {
    RoundKeys w=expandKey(key);
    addRoundKey(state,w,0);
    if(trace) printState("AES - Trang thai sau AddRoundKey vong 0",state);
    for(int round=1;round<=10;round++){
        subBytes(state,SBOX_AES);
        if(trace) printState("Vong "+to_string(round)+" - SubBytes",state);
        shiftRows(state);
        if(trace) printState("Vong "+to_string(round)+" - ShiftRows",state);
        if(round!=10){
            mixColumns(state);
            if(trace) printState("Vong "+to_string(round)+" - MixColumns",state);
        }
        addRoundKey(state,w,round);
        if(trace) printState("Vong "+to_string(round)+" - AddRoundKey",state);
    }
    return state;
}
Block aesDecrypt(Block state, const Block& key) {
    RoundKeys w=expandKey(key);
    addRoundKey(state,w,10);
    for(int round=9;round>=0;round--){
        invShiftRows(state);
        subBytes(state,INV_SBOX_AES);
        addRoundKey(state,w,round);
        if(round!=0) invMixColumns(state);
    }
    return state;
}

// ================================= MAIN =================================
int main() {
    cout << "========== KIEM THU DES ==========\n";
    const uint64_t desKey=parseHex64("133457799BBCDFF1");
    const uint64_t desPlain=parseHex64("0123456789ABCDEF");
    const uint64_t desExpected=parseHex64("85E813540F0AB405");
    cout << "Khoa:       " << hex64(desKey) << "\n";
    cout << "Plaintext:  " << hex64(desPlain) << "\n";
    cout << "\nLich khoa DES (16 khoa vong):\n";
    vector<uint64_t> keys=desKeys(desKey,true);
    cout << "\nCac trang thai DES:\n";
    uint64_t desCipher=desBlock(desPlain,keys,true);
    cout << "Ciphertext: " << hex64(desCipher) << "\n";
    cout << "Mong doi:   " << hex64(desExpected) << "\n";
    cout << "Kiem tra ma hoa: " << (desCipher==desExpected ? "PASS" : "FAIL") << "\n";
    reverse(keys.begin(),keys.end());
    uint64_t desBack=desBlock(desCipher,keys,false);
    cout << "Giai ma lai: " << hex64(desBack) << "\n";
    cout << "Kiem tra giai ma: " << (desBack==desPlain ? "PASS" : "FAIL") << "\n\n";

    cout << "========== KIEM THU AES-128 ==========\n";
    Block aesKey=parseHex128("000102030405060708090A0B0C0D0E0F");
    Block aesPlain=parseHex128("00112233445566778899AABBCCDDEEFF");
    Block aesExpected=parseHex128("69C4E0D86A7B0430D8CDB78070B4C55A");
    cout << "Khoa:       " << hexBytes(aesKey) << "\n";
    cout << "Plaintext:  " << hexBytes(aesPlain) << "\n\n";
    Block aesCipher=aesEncrypt(aesPlain,aesKey,true);
    cout << "Ciphertext: " << hexBytes(aesCipher) << "\n";
    cout << "Mong doi:   " << hexBytes(aesExpected) << "\n";
    cout << "Kiem tra ma hoa: " << (aesCipher==aesExpected ? "PASS" : "FAIL") << "\n";
    Block aesBack=aesDecrypt(aesCipher,aesKey);
    cout << "Giai ma lai: " << hexBytes(aesBack) << "\n";
    cout << "Kiem tra giai ma: " << (aesBack==aesPlain ? "PASS" : "FAIL") << "\n";

    if(desCipher==desExpected && desBack==desPlain && aesCipher==aesExpected && aesBack==aesPlain)
        cout << "\nKET LUAN: TAT CA KIEM THU DEU PASS.\n";
    else
        cout << "\nKET LUAN: CO KIEM THU THAT BAI - CAN KIEM TRA LAI.\n";
    return 0;
}
