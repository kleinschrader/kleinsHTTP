#include "httpServer.h"

 std::map<std::string,std::string> kleins::httpServer::mimeLookup = {
        {".html","text/html"},
        {".css","text/css"},
        {".js","text/javascript"},
        {".svg","image/svg+xml"}
        };

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

    std::async(std::launch::async,[conn](){
        conn->join();
        delete conn;
    });
}

void kleins::httpServer::on(const std::string& method,const std::string& uri, const std::function<void(httpParser*)> callback)
{
    std::string ref;
    ref.reserve(method.length() + uri.length() + 1);

    ref = method;
    ref.append(uri);

    functionTable.insert(std::make_pair(ref,callback));
}

void kleins::httpServer::serve(const std::string& uri, const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        std::cerr << "Error loading file " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    uint32_t filesize = std::filesystem::file_size(path);
    std::string filedata;

    filedata.resize(filesize);

    std::basic_ifstream<char> instream(path, std::ios::binary);

    instream.read(&filedata[0],filesize);

    fileLookup.insert(std::make_pair(uri, filedata));    

    

    on("GET",uri,[this](httpParser* parser){
        
        std::string extension = std::filesystem::path(parser->uri).extension();
        std::string mimetype = mimeLookup[extension];

        if(mimeLookup.find(extension) != mimeLookup.end())
        {
            parser->respond("200",{},fileLookup[parser->uri],mimetype);
        }
        else
        {
            parser->respond("200",{},fileLookup[parser->uri]);
        }
    });
}

void kleins::httpServer::serveDirectory(const std::string& baseuri, const std::string& path, const std::string indexFile)
{
    if(!std::filesystem::exists(path))
    {
        std::cerr << "Error loading directory " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    for(auto& p: std::filesystem::recursive_directory_iterator(path))
    {
        if(!p.is_regular_file())
            continue;

        std::string filepath = p.path();

        if(p.path().filename() == indexFile){
            serve(((std::string)p.path().parent_path()).append("/").substr(path.length()),p.path());
            
            std::string incompletePath = ((std::string)p.path().parent_path()).substr(path.length());
            if(incompletePath.length() != 0)
            {
                on("GET",incompletePath,[](httpParser* parser){
                    std::string locationHeader = "Location: " + parser->uri + "/";
                    parser->respond("301",
                        {locationHeader},
                        ""
                    );
                });
            }
        }
        else
        {
            serve(filepath.substr(path.length()),p.path());
        }
    }
}