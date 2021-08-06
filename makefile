LISTOFHEADERFILES= \
	source/connection/connection.h \
	source/httpParser/httpParser.h \
	source/packet/packet.h \
	source/socketBase/socketBase.h \
	source/tcpSocket/tcpSocket.h

TGT=target/release
TGTDBG=target/debug

BUILDPARAMS=g++ $^ -O2 -o $@ -c -fPIC 
BUILDPARAMS_DBG=g++ $^ -ggdb -o $@ -c -fPIC 


debug: $(TGTDBG)/libkleinsHTTP.so $(TGTDBG)/kleinsHTTP.h $(TGTDBG)/kleinsHTTP.a
	
	
release: $(TGT)/libkleinsHTTP.so $(TGT)/kleinsHTTP.h $(TGT)/kleinsHTTP.a

example: debug
	g++ -ggdb -o example.o example.cpp target/debug/kleinsHTTP.a -lpthread

gdb: example
	gdb example.o

$(TGTDBG)/libkleinsHTTP.so: $(TGTDBG)/connection.o $(TGTDBG)/httpParser.o $(TGTDBG)/packet.o $(TGTDBG)/socketBase.o $(TGTDBG)/tcpSocket.o
	g++ -shared -ggdb -o $@ $^

$(TGT)/libkleinsHTTP.so: $(TGT)/connection.o $(TGT)/httpParser.o $(TGT)/packet.o $(TGT)/socketBase.o $(TGT)/tcpSocket.o
	g++ -shared -O2 -o $@ $^

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

$(TGTDBG)/kleinsHTTP.a: $(TGTDBG)/connection.o $(TGTDBG)/httpParser.o $(TGTDBG)/packet.o $(TGTDBG)/socketBase.o $(TGTDBG)/tcpSocket.o
	ar rvs $@ $^

$(TGT)/kleinsHTTP.a: $(TGT)/connection.o $(TGT)/httpParser.o $(TGT)/packet.o $(TGT)/socketBase.o $(TGT)/tcpSocket.o
	ar rvs $@ $^

clean:
	rm -rf $(TGTDBG)/* || true
	rm -rf $(TGT)/* || true


###    DEBUG OBJECTS    ###

$(TGTDBG)/connection.o: source/connection/connection.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/packet.o: source/packet/packet.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/socketBase.o: source/socketBase/socketBase.cpp
	$(BUILDPARAMS_DBG)

$(TGTDBG)/tcpSocket.o: source/tcpSocket/tcpSocket.cpp
	$(BUILDPARAMS_DBG)

###    RELEASE OBJECTS    ###

$(TGT)/connection.o: source/connection/connection.cpp
	$(BUILDPARAMS)

$(TGT)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDPARAMS)

$(TGT)/packet.o: source/packet/packet.cpp
	$(BUILDPARAMS)

$(TGT)/socketBase.o: source/socketBase/socketBase.cpp
	$(BUILDPARAMS)

$(TGT)/tcpSocket.o: source/tcpSocket/tcpSocket.cpp
	$(BUILDPARAMS)