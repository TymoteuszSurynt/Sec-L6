#include <iostream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[]) {
    int client,portNum;
    char buffer[1024];
    char ip[] = "127.0.0.1";
    struct sockaddr_in server_addr;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(argc<3){
        cout<<"Error: Something is missing"<<endl;
        exit(1);
    }else{
        portNum=atoi(argv[1]);
    }
    if (client < 0){
        cout << "Error: establishing socket" << endl;
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cout << "- Connection to the server port number: " << portNum << endl;
    }
    recv(client, buffer, 1024, 0);
    cout << buffer << endl;
    cout << "- Connection confirmed, you are good to go!" << endl;
    strcpy(buffer,argv[3]);
    send(client, buffer, 1024, 0);
    cout << "Server says: ";
    recv(client, buffer, 1024, 0);
    cout << buffer << endl;
    close(client);
    return 0;
}