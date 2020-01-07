# Pretty Good Privacy(PGP)
PGP was invented by Phil Zimmermann developed in 1991 to provide emails with privacy, integrity, and authentication.  You can read more about it [here](https://en.wikipedia.org/wiki/Pretty_Good_Privacy)

### Prerequisites
1. C++ STL(Standard Template Library)
2. The Boost library. You can find more information about Boost [here](https://www.boost.org/). 
3. Openssl's crypto library

### About
This implementation contains the authentication implementation part of PGP. I have used C++ to implemnt this and has also made help of libraries like the C++ boost library and the openssl library. This has been implemented in a client-server fashion, where the client sends the message for authentication, and the server authenticates it.

### How to Run
To compile:
```
 g++ server.cpp  -lcrypto -o s
 g++ client.cpp  -lcrypto -o c
```
To Run
```
./s
./c
```