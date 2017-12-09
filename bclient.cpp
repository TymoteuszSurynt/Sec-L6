#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int option,size;
    ofstream publicKey,privateKey;
    cout << "Type: 1-Setup 2-Sign" << endl;
    cin>>option;
    if(option==1){
        cout<<"Choose size: 1-2048(default) 2-4096 3-8192 4-16384"<<endl;
        cin>>option;
        if(option==2){
            size=4096;
        }else if(option==3){
            size=8192;
        }else if(option==4){
            size=16384;
        }else{
            size=2048;
        }
        publicKey.open("publicKey.txt");
        privateKey.open("privateKey.txt");
        publicKey<<size;
        privateKey<<size;
        privateKey.close();
        publicKey.close();

    }
    return 0;
}