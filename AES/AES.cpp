#include <iostream> 
#include <bitset> 
#include <string> 
#include <fstream>

#include "utils.h"

using namespace std;

int main() {  

    string plainText, block, hexKey, binaryKey;
    cout << "Input plain text in HEX format : ";
    cin >> plainText;
    cout << endl;

    // ********** KEY RETRIEVAL AND EXPANSION ********** 
    // Reading the key from the file which is in hex format
    ifstream file("key.txt");
    if(file.is_open()) {
        getline(file, hexKey);
        cout << "********** KEY **********" << endl << endl;
        cout << "Key(HEX) : " << hexKey << endl << endl;
        // Converting hex key into binary format
        binaryKey = convertHexToBin(hexKey);
    }
    else {
        cout << "ERROR! \nUnable to open the file..!" << endl;
        exit(0);
    }

    // Now, for calculation purposes, the key has to be converted into an 
    // array of bytes. Also, for ease of calulation, we use the bitset data 
    // structure here.
    bitset<8> key[16];
    
    for(int j = 0, it = 0; j < nHex * 4; j += 8, it++) {
        string frac = binaryKey.substr(j, 8);
        key[it] = bitset<8>(frac);
    }
    // transpose(key, 4);

    // cout << "Key (State Matrix): \n\n";
    // printStateMatrix(key);

    cout << "********** KEY **********" << endl << endl;
    // ********** KEY RETRIEVAL AND EXPANSION ********** 


    // Taking 128-blocks at a time
    for(int i = 0, c = 0; i < plainText.size(); i = i + nHex, c++) {
        block = plainText.substr(i, nHex);
        // Padding the block when necessary
        if(block.size() != nHex) {
            while(block.size() != nHex) {
                block += '0';
            }
        }
        // Converting hex string into binary string
        string binaryPT = convertHexToBin(block);
        cout << "Input Block "<< c + 1 << " : " << block << endl << endl;

        // Now, for calculation purposes, the plain-text too has to be converted 
        // into an array of bytes. Here too, we use the bitset data structure.
        bitset<8> PT[16];
        for(int j = 0, it = 0; j < nHex*4; j += 8, it++) {
            string frac = binaryPT.substr(j, 8);
            PT[it] = bitset<8>(frac);
        }
        transpose(PT, 4);

        bitset<32> W[4 * (rounds + 1)]; 
        keyExpansion(key, W); 

        cout << endl << endl << "Plaintext to be encrypted:" << endl << endl; 
        printStateMatrix(PT);

        // ********** Encryption **********
        cout << "********** ENCRYPTION **********" << endl << endl;
        encryptAES(PT, W); 
        cout << "***** Cipher-text after Encryption of the Plain-text ***** " << endl; 
        printStateMatrix(PT);
        cout << endl;
        // ********** Encryption **********


        // ********** Decryption **********
        cout << "********** DECRYPTION **********" << endl << endl;
        decryptAES(PT, W); 
        cout << "***** Retrieved Plain-text after Decryption of the Cipher-text *****"<< endl;  
        printStateMatrix(PT);
        // ********** Decryption **********

    }

    return 0;  
} 