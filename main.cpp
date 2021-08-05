#include <iostream>
#include "source/socket/socket.h"
#include "source/connection/connection.h"
#include "source/httpParser/httpParser.h"

#include "source/packet/packet.h"

int main()
{
    std::cout << "Hello world!" << std::endl;
    kleins::tcpSocket socket("127.0.0.1",8080);

    auto socketFuture = socket.init();

    socket.newConnectionCallback = [](kleins::connection* conn){
        
        conn->onRecieveCallback = [conn](std::unique_ptr<kleins::packet> packet){
            std::unique_ptr<kleins::httpParser> parser = std::make_unique<kleins::httpParser>(packet.get(),conn);

            parser.get()->on("GET","/", [](kleins::httpParser* data){
                data->respond("200",{
                    "content-type:text/html; charset=UTF-8",
                },"Hello!");
            });

            if(!parser.get()->parse())
            {
                std::cout << "Error parsing http" << std::endl;
            }
        };

        conn->startOwnTickLoop();
        conn->join();

        delete conn;
    };

    if(!socketFuture.get())
    {
        return 1;
    };
    
    std::cin.get();
    return 0;
}