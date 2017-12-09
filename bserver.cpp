#include <iostream>
#include<openssl/rsa.h>
#include <openssl/pem.h>
#include <crypt.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
    int option, size, passwordSize = 12, saltSize = 4;
    string password, salt, password2;
    BIGNUM *bn;
    RSA *key;
    BIO *publicKey, *privateKey;
    unsigned long e = RSA_F4;

    cout << "Type: 1-Setup 2-Sign(default)" << endl;
    cin >> option;
    if (option == 1) {
        ofstream config;
        cout << "Choose size: 1-2048(default) 2-4096 3-8192 4-16384" << endl;
        cin >> option;
        if (option == 2) {
            size = 4096;
        } else if (option == 3) {
            size = 8192;
        } else if (option == 4) {
            size = 16384;
        } else {
            size = 2048;
        }
        cout << "1-Own password 2-Generated Password(default)" << endl;
        cin >> option;
        config.open("config");
        if (option == 1) {
            cout << "Enter password:" << endl;
            cin >> password;
        } else {
            srand(time(NULL));
            for (int i = 0; i < passwordSize; i++) {
                password += (char) (rand() % 105 + 21);
            }
            cout << "Your password:" << endl;
            cout << password << endl;
            cout << "Do not forget your password" << endl;

        }
        for (int i = 0; i < saltSize; ++i) {
            salt += (char) (rand() % 105 + 21);
        }
        password = crypt(password.c_str(), ("$6$" + salt).c_str());
        config << password << endl << salt;
        config.close();
        bn = BN_new();
        if (BN_set_word(bn, e) != 1) {
            cout << "Something went wrong!" << endl;
            exit(1);
        }
        key = RSA_new();
        if (RSA_generate_key_ex(key, size, bn, nullptr) != 1) {
            cout << "Something went wrong!" << endl;
            exit(1);
        }
        publicKey = BIO_new_file("publicKey.pem", "w+");
        privateKey = BIO_new_file("privateKey.pem", "w+");
        if (PEM_write_bio_RSAPublicKey(publicKey, key) != 1) {
            cout << "Something went wrong!" << endl;
            exit(1);
        }
        if (PEM_write_bio_RSAPrivateKey(privateKey, key, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
            cout << "Something went wrong!" << endl;
            exit(1);
        }
        BIO_free_all(privateKey);
        BIO_free_all(publicKey);
        RSA_free(key);
        BN_free(bn);

    } else {
        ifstream config("config");
        if (config.is_open()) {
            getline(config, password);
            getline(config, salt);
            cout << "Enter password" << endl;
            cin >> password2;
            if (crypt(password2.c_str(), ("$6$" + salt).c_str()) == password) {
                int server, client; // socket file descriptors
                int portNum;
                string echo;
                char buffer[1024]; // buffer to transmit
                bool isExit = false; // var fo continue infinitly
                struct sockaddr_in server_addr;
                socklen_t size;


                if(argc<2){
                    cout<<"No port added, running on default 100"<<endl;
                    portNum=100;
                }else{
                    portNum=atoi(argv[1]);
                }
                server = socket(AF_INET, SOCK_STREAM, 0);
                if (server < 0) {
                    cout << "Error: socket" << endl;
                    exit(1);
                }
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = htons(INADDR_ANY);
                server_addr.sin_port = htons(portNum);
                int yes = 1;
                if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
                    cout<<"Error: setsockopt"<<endl;
                    exit(1);
                }
                if ((bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
                    cout<< "Error: bind" << endl;
                    exit(1);
                }
                size = sizeof(server_addr);
                listen(server, 1);
                client = accept(server, (struct sockaddr *) &server_addr, &size);
                if (client < 0) {
                    cout << "Error: accept" << endl;
                }

                while (client > 0) {
                    strcpy(buffer, "Connected");
                    send(client, buffer, 1024, 0);
                    do {
                        cout << "\nClient: ";
                        echo = "";
                        do {
                            // wait the request from client
                            recv(client, buffer, 1024, 0);
                            cout << buffer << " ";
                            if (*buffer == '#') {
                                // exit loop and say goodbye (It's a polite server :D)
                                isExit = true;
                                *buffer = '*';
                                echo = "Goodbye!";
                            } else if ((*buffer != '#') && (*buffer != '*')) {
                                // concatenate the echo string to response to the client
                                echo += buffer;
                                echo += " ";
                            }
                        } while (*buffer != '*');
                        // copy the echo string to the buffer
                        sprintf(buffer, "%s", echo.c_str());
                        // send the message to the client
                        send(client, buffer, 1024, 0);
                    } while (!isExit);
                    cout << "\n\n=> Connection terminated with IP "
                         << inet_ntoa(server_addr.sin_addr);
                    close(client);
                    cout << "\nGoodbye..." << endl;
                    exit(1);

                }
                close(server);

            } else {
                cout << "Wrong password!" << endl;
            }
            config.close();
        }
    }
    return 0;
}