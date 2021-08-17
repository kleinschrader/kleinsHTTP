LISTOFHEADERFILES= \
	source/connectionBase/connectionBase.h \
	source/tcpConnection/tcpConnection.h \
	source/httpParser/httpParser.h \
	source/packet/packet.h \
	source/socketBase/socketBase.h \
	source/tcpSocket/tcpSocket.h \
	source/httpServer/httpServer.h \
	source/sslSocket/sslSocket.h \
	source/sslConnection/sslConnection.h

TGT=target/release
TGTDBG=target/debug

LIBS=-lssl -lcrypto -lpthread

BUILDPARAMS=g++ -std=c++17 $^ -O2 -o $@ -c -fPIC 
BUILDPARAMS_DBG=g++ -std=c++17 $^ -ggdb -o $@ -c -fPIC

VERSION=0.1.0


debug: $(TGTDBG)/libkleinsHTTP.so $(TGTDBG)/kleinsHTTP.h $(TGTDBG)/kleinsHTTP.a
	
	
release: $(TGT)/libkleinsHTTP.so $(TGT)/kleinsHTTP.h $(TGT)/kleinsHTTP.a

package: release
	tar -czvf $(TGT)/kleinsHTTP-v$(VERSION).tar.gz $(TGT)/kleinsHTTP.h $(TGT)/kleinsHTTP.a $(TGT)/libkleinsHTTP.so

example: debug example-cert
	g++ -std=c++17 -ggdb -o example.o example.cpp target/debug/kleinsHTTP.a -lpthread -lssl -lcrypto

example-cert:
	openssl req -new -x509 -subj "/C=DE/ST=Hamburg/L=Hamburg/O=kleinsHTTP DEV/OU=DEV/CN=localhost/emailAddress=kleinsHTTP@example.com" -days 365 -nodes -out example.crt -keyout example.key

gdb: example
	gdb example.o

$(TGTDBG)/libkleinsHTTP.so: $(TGTDBG)/connectionBase.o $(TGTDBG)/tcpConnection.o $(TGTDBG)/httpParser.o $(TGTDBG)/packet.o $(TGTDBG)/socketBase.o $(TGTDBG)/tcpSocket.o $(TGTDBG)/httpServer.o $(TGTDBG)/sslSocket.o $(TGTDBG)/sslConnection.o
	g++ -std=c++17 $(LIBS) -shared -ggdb -o $@ $^

$(TGT)/libkleinsHTTP.so: $(TGT)/connectionBase.o $(TGT)/tcpConnection.o $(TGT)/httpParser.o $(TGT)/packet.o $(TGT)/socketBase.o $(TGT)/tcpSocket.o $(TGT)/httpServer.o ${TGT}/sslSocket.o $(TGT)/sslConnection.o
	g++ -std=c++17 $(LIBS) -shared -O2 -o $@ $^

$(TGTDBG)/kleinsHTTP.h: preheader.h $(LISTOFHEADERFILES)
	rm $@ || true
	for file in $^; do \
		cat $${file} | grep -vE "\#include \"" >> $@; \
		echo >> $@; \
	done

$(TGT)/kleinsHTTP.h: preheader.h $(LISTOFHEADERFILES)
	rm $@ || true
	for file in $^; do \
		cat $${file} | grep -vE "\#include \"" >> $@; \
		echo >> $@; \
	done

$(TGTDBG)/kleinsHTTP.a: $(TGTDBG)/connectionBase.o $(TGTDBG)/tcpConnection.o $(TGTDBG)/httpParser.o $(TGTDBG)/packet.o $(TGTDBG)/socketBase.o $(TGTDBG)/tcpSocket.o $(TGTDBG)/httpServer.o $(TGTDBG)/sslSocket.o $(TGTDBG)/sslConnection.o
	ar rvs $@ $^

$(TGT)/kleinsHTTP.a:  $(TGT)/connectionBase.o $(TGT)/tcpConnection.o $(TGT)/httpParser.o $(TGT)/packet.o $(TGT)/socketBase.o $(TGT)/tcpSocket.o $(TGT)/httpServer.o $(TGT)/sslSocket.o $(TGT)/sslConnection.o
	ar rvs $@ $^

clean:
	rm -rf $(TGTDBG)/* || true
	rm -rf $(TGT)/* || true


###    DEBUG OBJECTS    ###

$(TGTDBG)/connectionBase.o: source/connectionBase/connectionBase.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/tcpConnection.o: source/tcpConnection/tcpConnection.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/packet.o: source/packet/packet.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/socketBase.o: source/socketBase/socketBase.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/tcpSocket.o: source/tcpSocket/tcpSocket.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/httpServer.o: source/httpServer/httpServer.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/sslSocket.o: source/sslSocket/sslSocket.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/sslConnection.o: source/sslConnection/sslConnection.cpp
	$(BUILDPARAMS_DBG)
###    RELEASE OBJECTS    ###

$(TGT)/connectionBase.o: source/connectionBase/connectionBase.cpp
	$(BUILDPARAMS)

$(TGT)/tcpConnection.o: source/tcpConnection/tcpConnection.cpp
	$(BUILDPARAMS)

$(TGT)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDPARAMS)

$(TGT)/packet.o: source/packet/packet.cpp
	$(BUILDPARAMS)

$(TGT)/socketBase.o: source/socketBase/socketBase.cpp
	$(BUILDPARAMS)

$(TGT)/tcpSocket.o: source/tcpSocket/tcpSocket.cpp
	${BUILDPARAMS}

$(TGT)/httpServer.o: source/httpServer/httpServer.cpp
	$(BUILDPARAMS)

$(TGT)/sslSocket.o: source/sslSocket/sslSocket.cpp
	$(BUILDPARAMS)

$(TGT)/sslConnection.o: source/sslConnection/sslConnection.cpp
	$(BUILDPARAMS)