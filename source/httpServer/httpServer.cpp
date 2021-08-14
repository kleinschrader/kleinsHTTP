#include "httpServer.h"

kleins::httpServer::httpServer(/* args */)
{
}

kleins::httpServer::~httpServer()
{
    sockets.clear();
}

bool kleins::httpServer::addSocket(socketBase* socket)
{
    sockets.push_back(
        std::unique_ptr<socketBase>(socket)
    );

    auto socketInitFuture = socket->init();

    socket->newConnectionCallback = [this](connectionBase* conn){
        if(conn->getAlive())
        {
            this->newConnection(conn);
        } 
    };

    if(socketInitFuture.get())
    {
       socket->startTicks();
       return true;
    }

    return false;
}

void kleins::httpServer::newConnection(kleins::connectionBase* conn) {

    conn->onRecieveCallback = [this,conn](std::unique_ptr<kleins::packet> packet){
        auto parser = std::unique_ptr<kleins::httpParser>(new kleins::httpParser(packet.get(),conn));

        for(auto cb : this->functionTable)
        {
            parser->on(cb.first,cb.second);
        }

        parser.get()->parse();

        conn->close_socket();
    };

    if(conn->onRecieveCallback)
    {
        conn->startOwnTickLoop();
    }

    conn->join();
    delete conn;
}

void kleins::httpServer::on(const std::string& method,const std::string& uri, const std::function<void(httpParser*)> callback)
{
    std::string ref;
    ref.reserve(method.length() + uri.length() + 1);

    ref = method;
    ref.append(uri);

    functionTable.insert(std::make_pair(ref,callback));
}