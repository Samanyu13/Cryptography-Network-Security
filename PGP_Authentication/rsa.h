#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/multiprecision/cpp_int.hpp>


using namespace boost::multiprecision;
using namespace std;


/// hexadecimal to binary conversion - with a facility of adding an offset; defaults to zero when no offset provided
string hexNumToBin(string s, int offset = 0) {
	unordered_map<string, string> mp; 
	mp[to_string(offset + 0)] = "0000"; 
	mp[to_string(offset + 1)] = "0001"; 
	mp[to_string(offset + 2)] = "0010"; 
	mp[to_string(offset + 3)] = "0011"; 
	mp[to_string(offset + 4)] = "0100"; 
	mp[to_string(offset + 5)] = "0101"; 
	mp[to_string(offset + 6)] = "0110"; 
	mp[to_string(offset + 7)] = "0111"; 
	mp[to_string(offset + 8)] = "1000"; 
	mp[to_string(offset + 9)] = "1001"; 
	mp[to_string(offset + 10)] = "1010"; 
	mp[to_string(offset + 11)] = "1011"; 
	mp[to_string(offset + 12)] = "1100"; 
	mp[to_string(offset + 13)] = "1101"; 
	mp[to_string(offset + 14)] = "1110"; 
	mp[to_string(offset + 15)] = "1111"; 

	string bin = "";
	for(int i = 0; i < s.size(); i += 2) {
        string substring = s.substr(i, 2);
		bin += mp[substring]; 
	} 
	return bin; 
}

/// binary to hexadecimal conversion - with a facility of adding an offset; defaults to zero when o offset provided
string binToHexNum(string s, int offset = 0) {
	unordered_map<string, string> mp; 
	mp["0000"] = to_string(offset + 0); 
	mp["0001"] = to_string(offset + 1); 
	mp["0010"] = to_string(offset + 2); 
	mp["0011"] = to_string(offset + 3); 
	mp["0100"] = to_string(offset + 4); 
	mp["0101"] = to_string(offset + 5); 
	mp["0110"] = to_string(offset + 6); 
	mp["0111"] = to_string(offset + 7); 
	mp["1000"] = to_string(offset + 8); 
	mp["1001"] = to_string(offset + 9); 
	mp["1010"] = to_string(offset + 10); 
	mp["1011"] = to_string(offset + 11); 
	mp["1100"] = to_string(offset + 12); 
	mp["1101"] = to_string(offset + 13); 
	mp["1110"] = to_string(offset + 14); 
	mp["1111"] = to_string(offset + 15);

	string hex = "";
	for(int i = 0; i < s.length(); i += 4) {
		string substring = s.substr(i, 4);
		hex += mp[substring]; 
	} 
	return hex; 
}

string convertStringToBinary(string s) {

    string binary_form = "";
    for(int i = 0; i < s.size(); i++) {
        char ch = s[i];

        for(int j = 7;j >= 0; j--) {
            if (ch & (1 << (j))) {
                binary_form += "1";
            }
            else {
                binary_form += "0";
            }
        }
    }
    return binary_form;
}

string convertHexNumToBin(cpp_int s, int offset) {
    string input = s.convert_to<string>();
    string result = "";
    result = hexNumToBin(input, offset);
    return result;
}


string convertBinaryToString(string s) {
    string output;
    for(int i = 0; i < s.size(); i += 8) {
        string substring = s.substr(i, 8);
        char ch = 0;
        for(int k = 7; k >= 0; k--) {
            if(substring[7 - k] == '1') {
                ch |= (1 << k);
            }
        }
        output += ch;
    }
    return output;
}

cpp_int inverse(cpp_int a, cpp_int b) {
    cpp_int q, r, t, t1, t2, r1, r2;
    r1 = a;
    r2 = b;
    t1 = 0;
    t2 = 1;
    while(r2 > 0) {
        q = r1 / r2;
        r = r1 - q * r2;
        r1 = r2;
        r2 = r;
        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }
    if(r1 == 1) {
        if(t1 < 0) {
            return t1+a;
        }
        return t1;
    }
    return 0;
}

unsigned bitlen(cpp_int a) {
    unsigned len = 0;
    while(a > 0)
    {
        a = a / 2;
        len++;
    }
    return len;
}

cpp_int repeated_squaring(cpp_int plain_text, cpp_int e, cpp_int n) {
    unsigned t = bitlen(e);   
    cpp_int b = 1, A = plain_text;
    if(t == 0) {
        return b;
    }

    if(bit_test(e,0)) {
        b = plain_text;
    }

    for(unsigned i = 1; i <= t; i++) {
        A = (A * A) % n;
        if(bit_test(e, i)) {
            b = (A * b) % n;
        }
    }
    return b;
}


string addEndLine(string a, int maxLen) {
    string result = "";
    for(int i = 0; i < a.size(); i++) {
        if(i % maxLen != 0) {
            result+=a[i];
        }
        else {
            result += "\n";
        }        
    }
    return result;
}