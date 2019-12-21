#include "iostream"
#include "string"
#include "boost/multiprecision/cpp_int.hpp"
#include "rsa.h"

#define offset 13

using namespace std;
using namespace boost::multiprecision;

unsigned bitlen(cpp_int a);

int main() {

    string retrieved_binary, binary_pt;

    cpp_int p, q, n, phi, e, d, cipher, m, converted_pt;

    //large prime numbers p and q
    p = cpp_int("12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803541");
    q = cpp_int("12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415233");

    string plain_text = "Hello";
    // cout << "Enter the plain text to be encrypted: ";
    // cin >> plain_text;

    binary_pt = convertStringToBinary(plain_text);
    cout << endl << "The plain text to binary: " << binary_pt << endl;
    converted_pt = cpp_int(binToHexNum(binary_pt, offset));

    cout << "The \"numberised\" plain text: " << converted_pt;

    //finding out the values of n, phi(n), e and d
    phi = (p - 1) * (q - 1);
    n = p * q;
    for(cpp_int i = phi - 1; i > 1; i--) {
        if(__gcd(i,phi) == 1) {
            e = i;
            break;
        }
    }
    d = inverse(phi, e);

    //obtaining the encrypted text
    cipher = repeated_squaring(converted_pt, e, n);

    cout << "\n\nThe encrypted message: " << cipher << endl << endl;

    cout << "\n\n*********************************RSA Public Key*********************************" << endl;
    string public_k = e.convert_to<string>();
    public_k += n.convert_to<string>();
    public_k = addEndLine(public_k, 81);
    cout << public_k << endl;

    cout << "\n\n********************************RSA Private Key*********************************" << endl;
    string private_k = d.convert_to<string>();
    private_k += n.convert_to<string>();
    private_k = addEndLine(private_k, 81);
    cout << private_k << endl << endl;

    //obtaining back the plain text
    m = repeated_squaring(cipher, d, n);
    cout << endl << "Decrypted numberised text: " << m << endl;
    retrieved_binary = convertHexNumToBin(m, offset);

    cout << endl << "Retrieved binary: " << retrieved_binary << endl;

    if(binary_pt == retrieved_binary) {
        cout << "Encryption and Decryption successful..!\n";
        string output = convertBinaryToString(retrieved_binary);
        cout << "The retrieved plain text: " << output << endl;
    }
    else {
        cout << "Oops..!..Something wrong with the algorithm..!" << endl;
    }

    return 0;
}

