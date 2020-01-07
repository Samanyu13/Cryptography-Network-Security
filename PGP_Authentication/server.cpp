#include "unistd.h" 
#include "sys/socket.h"
#include "stdlib.h"
#include "openssl/sha.h"
#include "boost/multiprecision/cpp_int.hpp"
#include "netinet/in.h" 
#include "string" 
#include "rsa.h"

#define PORT 8080
#define offset 13

using namespace std;

int main() {

 //**********Setting up the socket**********   
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024]; 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed");
        return -1;
	}
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) { 
		perror("bind failed"); 
        return -1;
	} 
	if (listen(server_fd, 3) < 0) { 
		perror("listen"); 
		return -1;
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) { 
		perror("accept"); 
		return -1;
	} 

//******************************************

    cpp_int p, q, n, phi, d, converted_pt, decrypted_hash, e;
    p = cpp_int("12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803541");
    q = cpp_int("12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415233");
    phi = (p - 1) * (q - 1);
    n = p * q;

    string cipher, plainText, hashInBinary, retrievedBinary, hash;

    //Receiving from the other side
   	read( new_socket, buffer, 1024);
    string input = buffer;
    cout << "Received from the other side: " << input << endl <<endl;

    // splitting into constituent parts using delimiter
    cipher = input.substr(0, input.find("<./>"));
    plainText = input.substr(input.find("<./>") + 4);
    plainText = plainText.substr(0,plainText.find("<<>>"));
    cout << "Retrieved cipher: " << cipher << endl << endl; 
    cout << "Retrieved plaintext: " << plainText << endl << endl;

    //hashing the retrieved plaintext using SHA-1
    unsigned char temp_hash[20];
    unsigned char ppp[plainText.size()];
    for(int x = 0; x < plainText.size(); x++) {
        ppp[x] = plainText[x];
    }
    ppp[plainText.length()]='\0';
    SHA1(ppp, plainText.length(), temp_hash);

    //saving hash back to a string type for easy operations
    hash = "";
    for(int i = 0; i < 20; i++) {
        hash += temp_hash[i];
    }
    //generated hash in binary form
    cout << "Generated hash: " << hash << endl <<endl;

    hashInBinary = convertStringToBinary(hash);
    cout << endl << "The hash to binary: " << hashInBinary << endl;

    //Decryption
    for(cpp_int i = phi - 1; i > 1; i--) {
        if(__gcd(i, phi) == 1) {
            e = i;
            break;
        }
    }

    cout << "\n\n********************************RSA Public Key*********************************" << endl;
    string public_k = e.convert_to<string>();
    public_k += n.convert_to<string>();
    public_k = addEndLine(public_k, 81);
    cout << public_k << endl << endl;

    decrypted_hash = repeated_squaring(cpp_int(cipher), e, n);
    cout << "Decrypt: " << decrypted_hash << endl;
    retrievedBinary = convertHexNumToBin(decrypted_hash, offset);

    cout << "Retrieved binary:" << retrievedBinary << endl;

    if(retrievedBinary == hashInBinary) {
        cout << endl << "Received message \"" << plainText << "\" authenticated successfully..!!!";
    }
	return 0; 
} 
