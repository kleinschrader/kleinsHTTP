#include <iostream>
#include "target/debug/kleinsHTTP.h"

int main()
{
    std::cout << "Hello world!" << std::endl;
    
    kleins::httpServer server;

    server.printVersion();

    server.addSocket(new kleins::tcpSocket("0.0.0.0",8080));
    server.addSocket(new kleins::sslSocket("0.0.0.0",4433,"./example.crt","./example.key"));

    server.on(kleins::httpMethod::GET,"/",[](kleins::httpParser* data){
        data->respond("200",{},"Hello!");
    });

    server.on(kleins::httpMethod::GET,"/hello",[](kleins::httpParser* data){
        std::stringstream response;
        response << "Hello " << data->parameters["name"] << "! ";

        response << "This is " << data->headers["Host"];
        
        data->respond("200",{},response.str());
    });

    server.on(kleins::httpMethod::POST,"/hello",[](kleins::httpParser* data){
        std::stringstream response;
        response << "Hello " << data->parameters["name"] << "! ";

        response << "This is " << data->headers["Host"];
        
        data->respond("200",{},response.str());
    });

    std::cin.get();

    return 0;
}