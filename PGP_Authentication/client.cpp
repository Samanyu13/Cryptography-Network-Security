#include "iostream"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "openssl/sha.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "string" 
#include "rsa.h"

#define PORT 8080 
#define offset 13

int main() {

//**********Setting up the socket**********
	int sock = 0; 
	struct sockaddr_in serv_addr; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
//*****************************************

    string binary_hash, to_send;

    cpp_int p, q, n, phi, e, cipher, d, converted_hash;
    p = cpp_int("12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803541");
    q = cpp_int("12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415233");
    string plain_text = "Hello there";
    // cout << "Enter the plain text to be encrypted: ";
    // cin >> plain_text;

    unsigned char ppp[plain_text.length()];
    for(int x = 0; x < plain_text.length(); x++) {
        ppp[x] = plain_text[x];
    }
    ppp[plain_text.length()]='\0';

    //hashing using SHA-1
    unsigned char temp_hash[20];
    SHA1(ppp, plain_text.length(), temp_hash);

    cout << "The hash obtained:" << temp_hash << endl;

    //saving hash back to a string type for easy operations
    string hash = "";
    for(int i = 0; i < 20; i++) {
        hash += temp_hash[i];
    }

    binary_hash = convertStringToBinary(hash);
    cout << endl << "The hash to binary: " << binary_hash << endl;
    converted_hash = cpp_int(binToHexNum(binary_hash, offset));

    cout << "The \"numberised\" hash code: " << converted_hash;

    //finding out the values of n, phi(n), and e
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
    cipher = repeated_squaring(converted_hash, d, n);
    
    cout << "\n\nThe encrypted message: " << cipher << endl << endl;

    //to be sent to the server for authentication 
    to_send = cipher.convert_to<string>() + "<./>" + plain_text + "<<>>";  
    send(sock, to_send.c_str(), to_send.size(), 0);
    cout << "Data sent to the other side: " << to_send << endl << endl;


    cout << "\n\n*********************************RSA Private Key(You are not supposed to print this :/)*********************************" << endl;
    string private_k = d.convert_to<string>();
    private_k += n.convert_to<string>();
    private_k = addEndLine(private_k, 81);
    cout << private_k << endl;
 
	return 0; 
} 
