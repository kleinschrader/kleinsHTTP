#include "../../libkleinsHTTP.h"

int main(){
    kleins::httpServer server;

    server.printVersion();

    server.addSocket(new kleins::tcpSocket("0.0.0.0",8080));

    server.on(kleins::httpMethod::GET,"/",[](kleins::httpParser* data){
        data->respond("200",{},"Hello!");
    });

    for(;;) {
        usleep(90000);
    }
}
