#include "httpServer.h"
#include "../metricsServer/metricsServer.h"

std::map<std::string, std::string> kleins::httpServer::mimeLookup = {

    /*
    * Common mime types
    * Source: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
    */

    /* AAC Audio                    */ {".aac", "audio/aac"},
    /* AbiWord document             */ {".abw", "application/x-abiword"},
    /* Archive document             */ {".arc", "application/x-freearc"},
    /* AVI: Audio Video Interleave  */ {".avi", "video/x-msvideo"},
    /* Amazon Kindle eBook format   */ {".azw", "application/vnd.amazon.ebook"},
    /* Any kind of binary data      */ {".bin", "application/octet-stream"},
    /* Windows OS/2 Bitmap Graphics */ {".bmp", "image/bmp"},
    /* BZip archive                 */ {".bz", "application/x-bzip"},
    /* BZip2 archive                */ {".bz2", "application/x-bzip"},
    /* CD audio                     */ {".cda", "application/x-cdf"},
    /* C-Shell script               */ {".csh", "application/x-csh"},
    /* CSS stylesheet               */ {".css", "text/css"},
    /* Comma seperated data         */ {".csv", "text/csv"},
    /* Microsoft Word               */ {".doc", "application/msword"},
    /* Microsoft Word (OpenXML)     */ {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    /* MS Embedded OpenType fonts   */ {".eot", "application/vnd.ms-fontobject"},
    /* Electronic publication       */ {".epub", "application/epub+zip"},
    /* GZip Compressed Archive      */ {".gz", "application/gzip"},
    /* It's pronounced 'gif'        */ {".gif", "image/gif"},
    /* Hypertext Markup Language    */ {".html", "text/html"},
    /* Icon format                  */ {".ico", "image/vnd.microsoft.icon"},
    /* iCalendar format             */ {".ics", "text/calendar"},
    /* Java Archive                 */ {".jar", "application/java-archive"},
    /* JPEG images                  */ {".jpg", "image/jpeg"},
    /* JPEG images                  */ {".jpeg", "image/jpeg"},
    /* JavaScript                   */ {".js", "text/javascript"},
    /* JSON format                  */ {".json", "application/json"},
    /* JSON-LD format               */ {".jsonld", "application/ld+json"},
    /* Musical Instrument Digital Interface */ {".mid", "audio/midi"}, // audio/x-midi is also listed for this file type. *shrugs*
    /* Musical Instrument Digital Interface */ {".midi", "audio/midi"},
    /* JavaScript module            */ {".mjs", "text/javascript"},
    /* MP3 audio                    */ {".mp3", "audio/mpeg"},
    /* MP4 audio                    */ {".mp4", "video/mp4"},
    /* MPEG Video                   */ {".mpeg", "video/mpeg"},
    /* Apple Installer Package      */ {".mpkg", "application/vnd.apple.installer+xml"},
    /* OpenDocument presentation document */ {".odp", "application/vnd.oasis.opendocument.presentation"},
    /* OpenDocument spreadsheet document  */ {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
    /* OpenDocument text document   */ {".odt", "application/vnd.oasis.opendocument.text"},
    /* OGG audio                    */ {".oga", "audio/ogg"},
    /* OGG audio                    */ {".ogg", "audio/ogg"},
    /* OGG video                    */ {".ogv", "video/ogg"},
    /* OGG                          */ {".ogx", "application/ogg"},
    /* Opus audio                   */ {".opus", "audio/opus"},
    /* OpenType font                */ {".otf", "font/otf"},
    /* Portable Network Graphics    */ {".png", "image/png"},
    /* Adobe Portable Document      */ {".pdf", "application/pdf"},
    /* Hypertext Preprocessor       */ {".php", "application/x-httpd-php"},
    /* Microsoft PowerPoint         */ {".ppt", "application/vnd.ms-powerpoint"},
    /* Microsoft PowerPoint (OpenXML) */ {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    /* RAR archive                  */ {".rar", "application/vnd.rar"},
    /* Rich Text Format             */ {".rtf", "application/rtf"},
    /* Bourne shell script          */ {".sh", "application/x-sh"},
    /* Scalable Vector Graphics     */ {".svg", "image/svg+xml"},
    /* Adobe Flash                  */ {".swf", "application/x-shockwave-flash"},
    /* Tape Archive                 */ {".tar", "application/x-tar"},
    /* Tagged Image File Format     */ {".tif", "image/tiff"},
    /* Tagged Image File Format     */ {".tiff", "image/tiff"},
    /* TrueType Font                */ {".ttf", "font/ttf"},
    /* Text, (generally ASCII)      */ {".txt", "text/plain"},
    /* Microsoft Visio              */ {".vsd", "application/vnd.visio"},
    /* Waveform Audio Format        */ {".wav", "audio/wav"},
    /* WEBM audio                   */ {".weba", "audio/webm"},
    /* WEBM video                   */ {".webm", "video/webm"},
    /* WEBP image                   */ {".webp", "image/webp"},
    /* Web Open Font Format         */ {".woff", "font/woff"},
    /* Web Open Font Format         */ {".woff2", "font/woff2"},
    /* XHTML                        */ {".xhtml", "application/xhtml+xml"},
    /* Microsoft Excel              */ {".xls", "application/vnd.ms-excel"},
    /* Microsoft Excel (OpenXML)    */ {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    /* XML                          */ {".xml", "application/xml"}, // If this doesn't work, try text/xml
    /* XUL                          */ {".xul", "application/vnd.mozilla.xul+xml"},
    /* Zip Archive                  */ {".zip", "application/zip"},
    /* 3GPP audio/video container   */ {".3pg", "video/3gpp"},  // audio/3gpp if it doesn't contain video
    /* 3GPP2 audio/video container  */ {".3g2", "video/3gpp2"}, // audio/3gpp2 if it doesn't contain video
    /* 7-zip archive                */ {".7z", "application/x-7z-compressed"}};

std::map<kleins::httpMethod, std::string> kleins::httpServer::methodLookup = {
    {kleins::httpMethod::GET, "GET"},         {kleins::httpMethod::HEAD, "HEAD"},     {kleins::httpMethod::POST, "POST"},
    {kleins::httpMethod::PUT, "PUT"},         {kleins::httpMethod::DELETE, "DELETE"}, {kleins::httpMethod::CONNECT, "CONNECT"},
    {kleins::httpMethod::OPTIONS, "OPTIONS"}, {kleins::httpMethod::TRACE, "TRACE"},   {kleins::httpMethod::PATCH, "PATCH"},
};

kleins::httpServer::httpServer(/* args */) {
  sessionCleanupThread = new std::thread(cleanUpSessionLoop, this);
}

kleins::httpServer::~httpServer() {
  if (mServer != 0) {
    delete mServer;
    delete metric_totalAcccess;
    delete metric_notfound;
    delete metric_totalSessions;
    delete metric_activeSessions;
  }

  keepRunning = false;

  sessionCleanupThread->join();
  delete sessionCleanupThread;

  sockets.clear();
}

bool kleins::httpServer::addSocket(socketBase* socket) {
  sockets.push_back(std::unique_ptr<socketBase>(socket));

  auto socketInitFuture = socket->init();

  socket->newConnectionCallback = [this](connectionBase* conn) {
    if (conn->getAlive()) {
      this->newConnection(conn);
    }
  };

  if (socketInitFuture.get()) {
    socket->startTicks();
    return true;
  }

  return false;
}

void kleins::httpServer::newConnection(kleins::connectionBase* conn) {

  conn->onRecieveCallback = [this, conn](std::unique_ptr<kleins::packet> packet) {
    auto parser = std::unique_ptr<kleins::httpParser>(new kleins::httpParser(packet.get(), conn, this));

    for (auto cb : this->functionTable) {
      parser->on(cb.first, cb.second);
    }

    parser.get()->parse();

    if (parser->headers["Connection"] != "keep-alive") {
      conn->close_socket();
    }
  };

  if (conn->onRecieveCallback) {
    conn->startOwnTickLoop();
  }

  std::thread([conn]() {
    conn->join();
    delete conn;
  }).detach();
}

void kleins::httpServer::on(httpMethod method, const std::string& uri, const std::function<void(httpParser*)> callback) {
  std::string ref;
  ref.reserve(methodLookup[method].length() + uri.length() + 1);

  ref = methodLookup[method];
  ref.append(uri);

  if (mServer) {

    std::string handler = "handler=\"" + uri + "\"";
    metric_access->addBucket(handler.c_str());

    functionTable.insert(std::make_pair(ref, [this, callback](httpParser* parser) {
      metric_totalAcccess->inc();
      std::string handler = "handler=\"" + parser->path + "\"";
      (*metric_access)[handler.c_str()]->inc();
      callback(parser);
    }));
    return;
  }
  functionTable.insert(std::make_pair(ref, callback));
}

void kleins::httpServer::serve(const std::string& uri, const std::string& path) {
  if (!std::filesystem::exists(path)) {
    std::cerr << "Error loading file " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  uint32_t filesize = std::filesystem::file_size(path);
  std::string filedata;

  filedata.resize(filesize);

  std::basic_ifstream<char> instream(path, std::ios::binary);

  instream.read(&filedata[0], filesize);

  fileLookup.insert(std::make_pair(uri, filedata));

  on(GET, uri, [this](httpParser* parser) {
    std::string extension = std::filesystem::path(parser->path).extension();
    std::string mimetype = mimeLookup[extension];

    if (mimeLookup.find(extension) != mimeLookup.end()) {
      parser->respond("200", {}, fileLookup[parser->path], mimetype);
    } else {
      parser->respond("200", {}, fileLookup[parser->path]);
    }
  });
}

void kleins::httpServer::serveDirectory(const std::string& baseuri, const std::string& path, const std::string indexFile) {
  if (!std::filesystem::exists(path)) {
    std::cerr << "Error loading directory " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto& p : std::filesystem::recursive_directory_iterator(path)) {
    if (!p.is_regular_file())
      continue;

    std::string filepath = p.path();

    if (p.path().filename() == indexFile) {
      serve(((std::string)p.path().parent_path()).append("/").substr(path.length()), p.path());

      std::string incompletePath = ((std::string)p.path().parent_path()).substr(path.length());
      if (incompletePath.length() != 0) {
        on(GET, incompletePath, [](httpParser* parser) {
          std::string locationHeader = "Location: " + parser->path + "/";
          parser->respond("301", {locationHeader}, "");
        });
      }
    } else {
      serve(filepath.substr(path.length()), p.path());
    }
  }
}

void kleins::httpServer::printVersion() {
  std::cout << "kleinsHTTP Build: " << BUILD_VERSION << std::endl;
}

void kleins::httpServer::cleanUpSessionLoop(httpServer* server) {
  while (server->keepRunning) {

    std::map<std::string, sessionBase*>::iterator it = server->sessions.begin();

    auto currentTime = std::chrono::system_clock::now();

    while (it != server->sessions.end()) {
      sessionBase* sb = it->second;

      if (sb->expireTime < currentTime) {
        server->sessions.erase(it);

        server->metric_activeSessions->set(server->metric_activeSessions->get() - 1);

        delete sb;
      }

      it++;
    }

    usleep(5000000);
  }
}

void kleins::httpServer::startMetricsServer(uint16_t port) {
  metric_totalAcccess = new metrics::counterMetric("total_accesses", "The total ammount of access done to this server");
  metric_access = new metrics::histogramMetric("access", "What urls were accessed");
  metric_notfound = new metrics::counterMetric("total_notfound", "The total ammount of 404 Erros");
  metric_activeSessions = new metrics::gaugeMetric("active_sessions", "The ammount of currently active sessions");
  metric_totalSessions = new metrics::counterMetric("total_sessions", "The total ammount of sessions");

  mServer = new metrics::metricsServer;
  mServer->addSocket(new kleins::tcpSocket("0.0.0.0", port));

  ((metrics::metricsServer*)mServer)->addMetric(metric_totalAcccess);
  ((metrics::metricsServer*)mServer)->addMetric(metric_access);
  ((metrics::metricsServer*)mServer)->addMetric(metric_notfound);
  ((metrics::metricsServer*)mServer)->addMetric(metric_activeSessions);
  ((metrics::metricsServer*)mServer)->addMetric(metric_totalSessions);
}