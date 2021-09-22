#include "httpParser.h"

kleins::httpParser::httpParser(packet* httpdata, connectionBase* conn, httpServer* srv) {
  data = httpdata;
  connsocket = conn;
  server = srv;
}

kleins::httpParser::~httpParser() {
}

bool kleins::httpParser::parse() {
  unsigned int nextline = 0;

  for (int i = 0; i < data->data.length(); i++) {
    if (i + 3 >= data->data.length())
      return false;

    if (data->data[i] != '\r')
      continue;

    if (data->data[i + 1] != '\n')
      continue;

    requestline = data->data.substr(0, i);

    nextline = i + 2;

    break;
  }

  for (int i = nextline; i < data->data.length(); i++) {
    if (i + 3 >= data->data.length())
      return false;

    if (data->data[i] != '\r')
      continue;

    if (data->data[i + 1] != '\n')
      continue;

    if (data->data[i + 2] != '\r')
      continue;

    if (data->data[i + 3] != '\n')
      continue;

    header = data->data.substr(nextline, i - nextline);

    nextline = i + 4;
    break;
  }

  body = data->data.substr(nextline);

  parseRequestline();
  parseHeaders();

  if (method == "POST") {
    if (headers["content-type"] == "application/x-www-form-urlencoded") {
      parseURLencodedData(body);
    }
  }

  std::string ref;

  ref = method;
  ref.append(path);

  auto search = functionTable.find(ref);
  if (search != functionTable.end()) {
    search->second(this);
  } else {
    if (server->metric_notfound) {
      server->metric_notfound->inc();
    }
    connsocket->sendData(
        "HTTP/1.0 404\r\ncontent-type:text/html; "
        "charset=UTF-8\r\nContent-Length: 51\r\n\r\n<html><head></head><body>Not "
        "found</body></html>\r\n",
        127);
  }

  return true;
}

void kleins::httpParser::on(const std::string& inmethod, const std::string& inuri, const std::function<void(kleins::httpParser*)> callback) {
  std::string ref;
  ref.reserve(inmethod.length() + inuri.length() + 1);

  ref = inmethod;
  ref.append(inuri);

  functionTable.insert(std::make_pair(ref, callback));
}

void kleins::httpParser::on(const std::string& ref, const std::function<void(kleins::httpParser*)> callback) {
  functionTable.insert(std::make_pair(ref, callback));
}

void kleins::httpParser::respond(
    const std::string& status, const std::list<std::string>& responseHeaders, const std::string& body, const std::string& mimeType) {
  std::stringstream response;

  response << "HTTP/1.1 " << status << "\r\n";

  std::list<std::string>::const_iterator it;
  for (it = responseHeaders.begin(); it != responseHeaders.end(); it++) {
    response << *it << "\r\n";
  }

  if (headers["Connection"] == "keep-alive") {
    response << "Keep-Alive: timeout=30\r\n";
  }

  response << "content-length: " << body.size() << "\r\n"
           << "Content-Type: " << mimeType << "; charset=utf-8 \r\n"
           << "Server: kleinsHTTP\r\n";

  if (sessionKey) {
    response << "Set-Cookie: KLEINSHTTP-SESSION=" << *sessionKey << "; SameSite=Strict; HttpOnly\r\n";
  };

  response << "\r\n" << body;

  std::string finalTarget;
  finalTarget = response.str();

  connsocket->sendData(finalTarget.c_str(), finalTarget.length());
}

void kleins::httpParser::parseRequestline() {
  const int length = requestline.length();

  char* bufferData = new char[(length + 1) * 3];

  char* methodBuffer = bufferData;
  char* pathBuffer = bufferData + ((length + 1) * 1);
  char* variableBuffer = bufferData + ((length + 1) * 2);

  memset(bufferData, 0, (length + 1) * 3);

  enum REQUESTLINE_STATES {
    REUQESTLINE_STATE_METHOD,
    REUQESTLINE_STATE_PATH,
    REQUESTLINE_STATE_PARAM,
    REQUESTLINE_STATE_UNPARSABLE,
  };

  int requestlineState = REUQESTLINE_STATE_METHOD;
  int offsetCounter = 0;
  for (auto x : requestline) {
    auto storeInBuffer = [x, &offsetCounter](char* b) { b[offsetCounter++] = x; };

    switch (requestlineState) {

    case REUQESTLINE_STATE_METHOD:

      if (x != ' ') {
        storeInBuffer(methodBuffer);
      } else {
        offsetCounter = 0;
        requestlineState = REUQESTLINE_STATE_PATH;
      }

      break;

    case REUQESTLINE_STATE_PATH:

      if (x != ' ' && x != '?') {
        storeInBuffer(pathBuffer);
      } else {
        offsetCounter = 0;
        requestlineState = REQUESTLINE_STATE_PARAM;
      }

      break;

    case REQUESTLINE_STATE_PARAM:

      if (x != ' ') {
        storeInBuffer(variableBuffer);
      } else {
        offsetCounter = 0;
        requestlineState = REQUESTLINE_STATE_UNPARSABLE;
      }

      break;
    default:
      break;
    }
  }

  parseURLencodedData(variableBuffer);

  method = methodBuffer;
  path = pathBuffer;

  delete[] bufferData;
}

void kleins::httpParser::parseURLencodedData(const char* rawData) {
  const int length = strlen(rawData);
  parseURLencodedData(rawData, length);
}

void kleins::httpParser::parseURLencodedData(std::string& rawData) {
  parseURLencodedData(rawData.c_str(), rawData.length());
}

void kleins::httpParser::parseURLencodedData(const char* rawData, const int length) {
  enum parseState {
    PARSE_STATE_KEY,
    PARSE_STATE_VALUE,
  };

  int offsetCounter = 0;
  uint8_t currentState = PARSE_STATE_KEY;

  char* buffer = new char[(length + 1) * 2];
  memset(buffer, 0, (length + 1) * 2);

  char* keyBuffer = buffer;
  char* valueBuffer = buffer + length + 1;

  for (int i = 0; i < length; i++) {
    switch (currentState) {

    case PARSE_STATE_KEY:
      if (rawData[i] != '=') {
        keyBuffer[offsetCounter++] = rawData[i];
      } else {
        offsetCounter = 0;
        currentState = PARSE_STATE_VALUE;
      }

      break;

    case PARSE_STATE_VALUE:
      if (rawData[i] != '&') {
        valueBuffer[offsetCounter++] = rawData[i];
      } else {
        offsetCounter = 0;
        currentState = PARSE_STATE_KEY;

        parameters.insert(std::make_pair(std::string(keyBuffer), std::string(valueBuffer)));

        memset(buffer, 0, (length + 1) * 2);
      }

      break;

    default:
      break;
    }
  }

  if (currentState == PARSE_STATE_VALUE) {
    parameters.insert(std::make_pair(std::string(keyBuffer), std::string(valueBuffer)));
  }

  delete[] buffer;
}

void kleins::httpParser::parseHeaders() {
  enum parseState {
    PARSE_STATE_KEY,
    PARSE_STATE_VALUE,
  };

  int length = header.length();

  int offsetCounter = 0;
  uint8_t currentState = PARSE_STATE_KEY;

  char* buffer = new char[(length + 1) * 2];
  memset(buffer, 0, (length + 1) * 2);

  char* keyBuffer = buffer;
  char* valueBuffer = buffer + length + 1;

  bool skipNext = false;
  for (auto x : header) {
    if (skipNext) {
      skipNext = false;
      continue;
    }

    switch (currentState) {
    case PARSE_STATE_KEY:

      if (x != ':') {
        keyBuffer[offsetCounter++] = x;
      } else {
        skipNext = true;
        offsetCounter = 0;
        currentState = PARSE_STATE_VALUE;
      }

      break;

    case PARSE_STATE_VALUE:

      if (x != '\x0D') {
        valueBuffer[offsetCounter++] = x;
      } else {
        skipNext = true;
        offsetCounter = 0;
        currentState = PARSE_STATE_KEY;

        headers.insert(std::make_pair(std::string(keyBuffer), std::string(valueBuffer)));

        memset(buffer, 0, (length + 1) * 2);
      }

      break;

    default:
      break;
    }
  }

  delete[] buffer;
}
