#include "httpParser.h"

kleins::httpParser::httpParser(packet* httpdata, connectionBase* conn)
{
    data = httpdata;
    connsocket = conn;
}

kleins::httpParser::~httpParser()
{
}

bool kleins::httpParser::parse()
{
    unsigned int nextline = 0; 
    
    for(int i = 0;i < data->data.length();i++) {
        if (i+3 >=  data->data.length())
            return false;

        if (data->data[i] != '\r')
            continue;

        if (data->data[i+1] != '\n')
            continue;

        requestline = data->data.substr(0,i);

        nextline = i+2;

        break;
    }

    for(int i = nextline;i < data->data.length();i++)
    {
        if (i+3 >=  data->data.length())
            return false;
    
        if (data->data[i] != '\r')
            continue;

        if (data->data[i+1] != '\n')
            continue;

        if (data->data[i+2] != '\r')
            continue;

        if (data->data[i+3] != '\n')
            continue;

        header = data->data.substr(nextline,i-nextline);

        nextline = i+4;
        break;
    }

    body = data->data.substr(nextline);

    for(int i = 0; i < requestline.length(); i++)
    {
        if(i >= requestline.length())
            return false;

        if(requestline[i] != ' ')
            continue;
        
        method = requestline.substr(0,i);

        nextline = i+1;

        break;
    }

    for(int i = nextline; i < requestline.length(); i++)
    {
        if(i >= requestline.length())
            return false;

        if(requestline[i] != ' ')
            continue;
        
        uri = requestline.substr(nextline,i - nextline);

        nextline = i+1;

        break;
    }

    std::string ref;
    ref.reserve(method.length() + uri.length() + 1);

    ref = method;
    ref.append(uri);

    auto search = functionTable.find(ref);
    if(search != functionTable.end())
    {
        search->second(this);
    }
    else
    {
        connsocket->sendData("HTTP/1.0 404\r\ncontent-type:text/html; charset=UTF-8\r\n\r\n<html><head></head><body>Not found</body></html>\r\n",104);
    }
    
    return true;
}

void kleins::httpParser::on(const std::string& inmethod,const std::string& inuri, const std::function<void(kleins::httpParser*)> callback)
{
    std::string ref;
    ref.reserve(inmethod.length() + inuri.length() + 1);

    ref = inmethod;
    ref.append(inuri);

    functionTable.insert(std::make_pair(ref,callback));
}

void kleins::httpParser::on(const std::string& ref, const std::function<void(kleins::httpParser*)> callback)
{
    functionTable.insert(std::make_pair(ref,callback));
}

void kleins::httpParser::respond(const std::string& status,const std::list<std::string>& responseHeaders,const std::string& body)
{
    std::stringstream response;
    
    response << "HTTP/1.1 " << status << "\r\n";

    std::list<std::string>::const_iterator it;
    for(it = responseHeaders.begin(); it != responseHeaders.end(); it++)
    {
        response << *it << "\r\n";
    }

    response << "content-length: " << body.size()+2 << "\r\n";
    response << "Content-Type: text/html; charset=utf-8" << "\r\n";
    response << "Server: kleinsHTTP" << "\r\n";


    response << "\r\n" << body << "\r\n";

    std::string finalTarget;
    finalTarget = response.str();
    
    connsocket->sendData(finalTarget.c_str(),finalTarget.length());
}