#include "../../libkleinsHTTP.h"

int main(){
    kleins::httpServer server;

    server.printVersion();

    server.addSocket(new kleins::sslSocket("0.0.0.0",4433,"example.crt","example.key"));

    server.on(kleins::httpMethod::GET,"/",[](kleins::httpParser* data){
        data->respond("200",{},"Hello!");
    });

    for(;;) {
        usleep(90000);
    }
}
