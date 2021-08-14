#include <iostream>
#include "target/debug/kleinsHTTP.h"
int main()
{
    std::cout << "Hello world!" << std::endl;
    
    kleins::httpServer server;
    server.addSocket(new kleins::tcpSocket("0.0.0.0",8080));
    server.addSocket(new kleins::sslSocket("0.0.0.0",4433,"./example.crt","./example.key"));

    server.on("GET","/",[](kleins::httpParser* data){
        data->respond("200",{},"Hello!");
    });

    std::cin.get();

    return 0;
}