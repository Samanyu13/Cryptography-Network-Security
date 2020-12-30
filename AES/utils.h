#include <iostream>
#include <string>
#include <ctype.h>
#include <bitset>
#include <unordered_map>

using namespace std;

/*
 *  Number of rounds for encryption in AES. rounds = 10 for AES-128.  
 */
const int rounds = 10; 
/*
 * Number of words that are input keys
 */
const int nWKey = 4; 
/* 
 * Number of HEX characters for 1 block. nHex = 32 for AES-128.
 */ 
const int nHex = 32;

/*
 * The substitution box.
 */
bitset<8> subBox[16][16] = { 
/*              00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F         */
/* 00 */    { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 }, 
/* 01 */    { 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 }, 
/* 02 */    { 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 }, 
/* 03 */    { 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 }, 
/* 04 */    { 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 }, 
/* 05 */    { 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF }, 
/* 06 */    { 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 }, 
/* 07 */    { 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 }, 
/* 08 */    { 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 }, 
/* 09 */    { 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB }, 
/* 0A */    { 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 }, 
/* 0B */    { 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 }, 
/* 0C */    { 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A }, 
/* 0D */    { 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E }, 
/* 0E */    { 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF }, 
/* 0F */    { 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 } 
}; 

/*
 * The inverse substitution box.
 */
bitset<8> invSubBox[16][16] = { 
/*              00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F         */
/* 00 */    {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB}, 
/* 01 */    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB}, 
/* 02 */    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E}, 
/* 03 */    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25}, 
/* 04 */    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92}, 
/* 05 */    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84}, 
/* 06 */    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06}, 
/* 07 */    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B}, 
/* 08 */    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73}, 
/* 09 */    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E}, 
/* 0A */    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B}, 
/* 0B */    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4}, 
/* 0C */    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F}, 
/* 0D */    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF}, 
/* 0E */    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61}, 
/* 0F */    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D} 
}; 

/*
 * Round constant, used in key expansion. (AES-128 only takes 10 rounds)
 */  
bitset<32> rCon[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
    0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000
};  

/*
 * Converts the given string (of alphabets) into its
 * corresponding HEX format such that A is mapped to 
 * 00, B to 01, ..., and Z to 25.
 */ 
string alphaStringToHex(string PT) {
    string CPT = "";
    
    for(int i = 0; i < PT.size(); i++) {
        int v = tolower(PT[i]) - 'a';
        if(v < 10) {
            CPT += '0';
            CPT += (char)('0' + v);
        }
        else {
            CPT += to_string(v);
        }
    }

    return CPT;
}

/* 
 * Finds the transpose of a nxn matrix in a 1D array
 */
void transpose(bitset<8> *PT, int n) {
    bitset<8> temp[n*n];
    int k = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            temp[k++] = PT[j * n + i];
        }
    }
    for(int i = 0; i < n*n; i++) {
        PT[i] = temp[i];
    }
}

/*
 * Converts a given hex string into its corresponding 
 * binary string.
 */
string convertHexToBin(string s) {
	unordered_map<char, string> map; 

	map['0'] = "0000"; 
	map['1'] = "0001"; 
	map['2'] = "0010"; 
	map['3'] = "0011"; 
	map['4'] = "0100"; 
	map['5'] = "0101"; 
	map['6'] = "0110"; 
	map['7'] = "0111"; 
	map['8'] = "1000"; 
	map['9'] = "1001"; 
	map['A'] = "1010"; 
	map['B'] = "1011"; 
	map['C'] = "1100"; 
	map['D'] = "1101"; 
	map['E'] = "1110"; 
	map['F'] = "1111"; 

	string bin = "";
    char ch;
	for(int i = 0; i < s.size(); i++) {
        if(isalpha(s[i])) {
            ch = toupper(s[i]);
        }
        else {
            ch = s[i];
        }
		bin += map[ch]; 
	} 
	return bin; 
}

void printStateMatrix(bitset<8> PT[16]) {
    for(int i = 0; i < 16; i++) { 
        cout << hex << uppercase << PT[i].to_ulong() << " "; 
        if((i + 1) % 4 == 0) {
            cout << endl; 
        }
    }  
    cout << endl; 
}

/* 
 * Convert four bytes to one word. 
 */  
bitset<32> Word(bitset<8>& k1, bitset<8>& k2, bitset<8>& k3, bitset<8>& k4) {  
    bitset<32> result(0x00000000);  
    bitset<32> temp;  
    temp = k1.to_ulong();  // K1  
    temp <<= 24;  
    result |= temp;  
    temp = k2.to_ulong();  // K2  
    temp <<= 16;  
    result |= temp;  
    temp = k3.to_ulong();  // K3  
    temp <<= 8;  
    result |= temp;  
    temp = k4.to_ulong();  // K4  
    result |= temp; 

    return result; 
}  
  
/* 
 * Cyclic left shift by 8 bits 
 * That is - [a0, a1, a2, a3] becomes [a1, a2, a3, a0] 
 * To left rotate by k bits, we can do as follows - 
 * return (n << k) | (n >> (size - k))
 */  
bitset<32> rotWord(bitset<32>& rotate) { 
    // cout << "RotWord: " << hex << uppercase << ((rotate << 8) | (rotate >> 24)).to_ulong() << endl; 
    return (rotate << 8) | (rotate >> 24);  
}  
  
/* 
 *  S-box transformation for each byte in input word 
 */  
bitset<32> subWord(bitset<32>& sw) {  
    bitset<32> temp;  
    for(int i = 0; i < 32; i += 8) {  
        int row = sw[i + 7] * 8 + sw[i + 6] * 4 + sw[i + 5] * 2 + sw[i + 4];  
        int col = sw[i + 3] * 8 + sw[i + 2] * 4 + sw[i + 1] * 2 + sw[i];  
        bitset<8> val = subBox[row][col];  
        for(int j = 0; j < 8; j++) {
            temp[i+j] = val[j]; 
        }  
    } 
    // cout << "SubWord: " << hex << uppercase << temp.to_ulong() << endl;
    return temp;  
}  
  
/*  
 *  Key Extension Function - Extended 128-bit key to w[4*(rounds+1)] 
 */   
void keyExpansion(bitset<8> key[4 * nWKey], bitset<32> w[4 * (rounds + 1)]) { 
    bitset<32> temp; 
    cout << "********** KEY EXPANSION **********\n";

    //The first four of w [] are input keys 
    for(int i = 0; i < nWKey; i++) {
        w[i] = Word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]); 
        cout << "w" << i << ": " << hex << uppercase << w[i].to_ulong() << "\t"; 
    }

    for(int i = nWKey; i < (4 * (rounds + 1)); i++) {
        temp = w[i - 1];
        if(i % 4 == 0) { 
            cout << "\n";
            bitset<32> rotW = rotWord(temp);
            temp = subWord(rotW) ^ rCon[i/nWKey - 1];
            // cout << "Rcon: " << hex << uppercase << rCon[i/nWKey - 1].to_ulong() << "\t"; 
        }
        w[i] = w[i - nWKey] ^ temp; 
        cout << dec << "w" << i << ": ";
        cout << hex << uppercase << w[i].to_ulong() << "\t"; 
    }
    cout << "\n\n********** KEY EXPANSION **********\n";

} 

/* 
 * Function to implement substitution with the help of 
 * the substitution box.
 */
void substituteBytes(bitset<8> pt[4 * 4]) { 
    for(int i = 0; i < 16; i++) { 
        int row = pt[i][7]*8 + pt[i][6]*4 + pt[i][5]*2 + pt[i][4];  
        int col = pt[i][3]*8 + pt[i][2]*4 + pt[i][1]*2 + pt[i][0];  
        pt[i] = subBox[row][col];  
    } 
    cout << endl << "After SubBytes\n"; 
    printStateMatrix(pt);
}  
  
/* 
 * A simple circular permutation function. 
 * The first row stays as such.
 * The second row is shifted one byte to the left.
 * The third row is shifted two bytes to the left.
 * Finally, the fourth row is shifted three bytes to the left.
 */  
void shiftRows(bitset<8> pt[4 * 4]) { 
    // Second row shift
    bitset<8> temp = pt[4];  
    for(int i = 0; i < 3; i++) {
        pt[i+4] = pt[i+5]; 
    } 
    pt[7] = temp; 

    //Third row shift
    for(int i = 0; i < 2; i++) { 
        temp = pt[i + 8]; 
        pt[i + 8] = pt[i + 10]; 
        pt[i + 10] = temp; 
    } 

    //Fourth row shift 
    temp = pt[15]; 
    for(int i = 3; i > 0; i--) {
        pt[i + 12] = pt[i + 11]; 
    }
    pt[12] = temp; 

    cout << "After ShiftRows\n";
    printStateMatrix(pt);
} 
  
/*
 * Multiplication over GF(2^8) 
 */  
bitset<8> mulGF(bitset<8> a, bitset<8> b) {   
    bitset<8> p = 0;  
    bitset<8> hi_bit_set;  
    for (int i = 0; i < 8; i++) {  
        if ((b & bitset<8>(1)) != 0) {  
            p ^= a;  
        }  
        hi_bit_set = (bitset<8>) (a & bitset<8>(0x80));  
        a <<= 1;  
        if (hi_bit_set != 0) {  
            a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */  
        }  
        b >>= 1;  
    }  
    return p;  
}  
  
/* 
 * Column transformation - Arithmetic over GF(2^8)
 */  
void mixColumns(bitset<8> pt[4 * 4]) { 
    bitset<8> temp[4]; 
    for(int i = 0; i < 4; i++) { 
        for(int j = 0; j < 4; j++) {
            temp[j] = pt[i + j * 4]; 
        }

        pt[i] = mulGF(0x02, temp[0]) ^ mulGF(0x03, temp[1]) ^ temp[2] ^ temp[3];  
        pt[i+4] = temp[0] ^ mulGF(0x02, temp[1]) ^ mulGF(0x03, temp[2]) ^ temp[3];  
        pt[i+8] = temp[0] ^ temp[1] ^ mulGF(0x02, temp[2]) ^ mulGF(0x03, temp[3]);  
        pt[i+12] = mulGF(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ mulGF(0x02, temp[3]);  
    } 

    cout << "After MixColumns\n";
    printStateMatrix(pt);
}  
  
/*
 *  Round Key Plus Transform - XOR each column with the extended key 
 */  
void addRoundKey(bitset<8> pt[4*4], bitset<32> k[4]) {  
    for(int i = 0; i < 4; i++) { 
        // Extracting the bytes off the key 
        bitset<32> k1 = k[i] >> 24; 
        bitset<32> k2 = (k[i] << 8) >> 24; 
        bitset<32> k3 = (k[i] << 16) >> 24; 
        bitset<32> k4 = (k[i] << 24) >> 24; 

        pt[i] = pt[i] ^ bitset<8>(k1.to_ulong()); 
        pt[i+4] = pt[i+4] ^ bitset<8>(k2.to_ulong()); 
        pt[i+8] = pt[i+8] ^ bitset<8>(k3.to_ulong()); 
        pt[i+12] = pt[i+12] ^ bitset<8>(k4.to_ulong()); 
    } 
    cout << "After AddRoundKey\n";
    printStateMatrix(pt);
}  

/* 
 * AES Encryption function
 */
void encryptAES(bitset<8> pt[4 * 4], bitset<32> w[4 * (rounds + 1)]) {  
    bitset<32> key[4]; 
    for(int i = 0; i < 4; i++) {
        key[i] = w[i]; 
    } 
    cout << "<<<<<<<< ROUND 0 >>>>>>>>\n\n";
    addRoundKey(pt, key);  
  
    for(int round = 1; round < rounds; round++) { 
    cout << "<<<<<<<< ROUND " << round << " >>>>>>>>\n";
        substituteBytes(pt);  
        shiftRows(pt);  
        mixColumns(pt);  
        for(int i = 0; i < 4; i++) {
            key[i] = w[4 * round + i]; 
        }
        addRoundKey(pt, key); 
    } 

    cout << "<<<<<<<< ROUND 10 >>>>>>>>\n";
    substituteBytes(pt); 
    shiftRows(pt);  
    for(int i = 0; i < 4; i++)  
        key[i] = w[4 * rounds + i];  
    addRoundKey(pt, key);  
}  

/* 
 *  Inverse S-box transformation 
 */  
void inverseSubstituteBytes(bitset<8> ct[4*4]) {  
    for(int i = 0; i < 16; i++) {  
        int row = ct[i][7] * 8 + ct[i][6] * 4 + ct[i][5] * 2 + ct[i][4];  
        int col = ct[i][3] * 8 + ct[i][2] * 4 + ct[i][1] * 2 + ct[i][0];  
        ct[i] = invSubBox[row][col];  
    } 
    cout << endl << "After InverseSubstituteBytes\n"; 
    printStateMatrix(ct);
} 
  
/* 
 *  Reverse Transform - Cyclic Right Shift in Bytes 
 */  
void inverseShiftRows(bitset<8> ct[4*4]) {  
    //The second line circle moves one bit to the right  
    bitset<8> temp = ct[7];  
    for(int i = 3; i > 0; i--) {
        ct[i + 4] = ct[i + 3]; 
    } 
    ct[4] = temp;  
    //The third line circle moves two to the right  
    for(int i = 0; i < 2; i++) {  
        temp = ct[i + 8];  
        ct[i + 8] = ct[i + 10];  
        ct[i + 10] = temp;  
    }  
    //Fourth line circle moves three to the right  
    temp = ct[12];  
    for(int i = 0; i < 3; i++) {
        ct[i+12] = ct[i+13]; 
    }   
    ct[15] = temp; 

    cout << endl << "After InverseShiftRows\n"; 
    printStateMatrix(ct);
} 

void inverseMixColumns(bitset<8> ct[4*4]) {  
    bitset<8> arr[4];  
    for(int i = 0; i < 4; i++) {  
        for(int j = 0; j < 4; j++) {
            arr[j] = ct[i+j*4]; 
        } 

        ct[i] = mulGF(0x0e, arr[0]) ^ mulGF(0x0b, arr[1]) ^ mulGF(0x0d, arr[2]) ^ mulGF(0x09, arr[3]); 
        ct[i + 4] = mulGF(0x09, arr[0]) ^ mulGF(0x0e, arr[1]) ^ mulGF(0x0b, arr[2]) ^ mulGF(0x0d, arr[3]); 
        ct[i + 8] = mulGF(0x0d, arr[0]) ^ mulGF(0x09, arr[1]) ^ mulGF(0x0e, arr[2]) ^ mulGF(0x0b, arr[3]); 
        ct[i + 12] = mulGF(0x0b, arr[0]) ^ mulGF(0x0d, arr[1]) ^ mulGF(0x09, arr[2]) ^ mulGF(0x0e, arr[3]); 
    } 
    cout << endl << "After InverseMixColumns\n"; 
    printStateMatrix(ct);
}  


/* 
 *  AES Decryption Function 
 */  
void decryptAES(bitset<8> ct[4*4], bitset<32> w[4*(rounds+1)]) { 
    bitset<32> key[4]; 
    for(int i = 0; i < 4; i++) { 
        key[i] = w[4 * rounds + i]; 
    }
    cout << "<<<<<<<< ROUND 0 >>>>>>>>\n\n";
    addRoundKey(ct, key); 
  
    for(int round = rounds - 1; round > 0; round--) { 
    cout << "<<<<<<<< ROUND " << rounds - round << " >>>>>>>>\n";
        inverseShiftRows(ct); 
        inverseSubstituteBytes(ct); 
        for(int i = 0; i < 4; i++) { 
            key[i] = w[4 * round + i]; 
        } 
        addRoundKey(ct, key); 
        inverseMixColumns(ct); 
    } 

    cout << "<<<<<<<< ROUND 10 >>>>>>>>\n";
    inverseShiftRows(ct); 
    inverseSubstituteBytes(ct); 
    for(int i = 0; i < 4; i++) { 
        key[i] = w[i]; 
    } 
    addRoundKey(ct, key); 
} 