#include "connection.h"

kleins::connection::connection(int connectionid)
{
    connectionfd = connectionid;
}

kleins::connection::~connection()
{
    close_socket();
    if(tickThread)
    {
        if(tickThread->joinable())
        {
            tickThread->join();
            delete tickThread;
        }
    }
}

void kleins::connection::startOwnTickLoop()
{
    if(!tickThread) 
    {
        tickThread = new std::thread(ownTickLoop, this);
    }
}

bool kleins::connection::getAlive()
{
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (connectionfd, SOL_SOCKET, SO_ERROR, &error, &len);

    return (error | retval) == 0;
}

void kleins::connection::ownTickLoop(connection* conn)
{
    while(conn->getAlive()) {
            conn->tick();
    }
}

void kleins::connection::tick()
{
    packet* packetBuffer = new packet;

    packetBuffer->data.resize(4096);
    packetBuffer->size = recv( connectionfd , (char*)&packetBuffer->data[0], 4096, MSG_DONTWAIT);
    
    if(packetBuffer->size == -1)
    {
        delete packetBuffer;
        usleep(20000);
        return;
    }

    packetBuffer->data.resize(packetBuffer->size);

    this->onRecieveCallback(std::unique_ptr<packet>(packetBuffer));
}

void kleins::connection::sendData(const char* data, int datalength)
{
    send(connectionfd, data, datalength, 0);
}

void kleins::connection::close_socket()
{
    close(connectionfd);
}

void kleins::connection::join()
{
    if(tickThread)
    {
        if(tickThread->joinable())
        {
            tickThread->join();
        }
    }
}