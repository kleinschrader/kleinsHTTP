BUILDCMD_DBG=g++ -ggdb -c -o $@ $^
BUILDCMD=g++ -O2 -c -o $@ $^
LIBS=-lpthread
TARGETDIR_DBG=target/debug
TARGETDIR=target/release

debug: $(TARGETDIR_DBG)/main.o $(TARGETDIR_DBG)/socket.o $(TARGETDIR_DBG)/connection.o $(TARGETDIR_DBG)/httpParser.o $(TARGETDIR_DBG)/packet.o
	g++ -ggdb $(LIBS) -o $(TARGETDIR_DBG)/glassline-backend-login $^

release: $(TARGETDIR)/main.o $(TARGETDIR)/socket.o $(TARGETDIR)/connection.o $(TARGETDIR)/httpParser.o $(TARGETDIR)/packet.o
	g++ $(LIBS) -O2 -o $(TARGETDIR)/glassline-backend-login $^
	strip $(TARGETDIR)/glassline-backend-login

gdb: $(TARGETDIR_DBG)/glassline-backend-login 
	gdb  $(TARGETDIR_DBG)/glassline-backend-login 

run: $(TARGETDIR_DBG)/glassline-backend-login
	$(TARGETDIR_DBG)/glassline-backend-login

$(TARGETDIR_DBG)/glassline-backend-login: debug

clean:
	rm $(TARGETDIR_DBG)/* || true
	rm $(TARGETDIR)/* || true

$(TARGETDIR_DBG)/main.o: main.cpp
	$(BUILDCMD_DBG)

$(TARGETDIR_DBG)/socket.o: source/socket/socket.cpp
	$(BUILDCMD_DBG)

$(TARGETDIR_DBG)/connection.o: source/connection/connection.cpp
	$(BUILDCMD_DBG)
	
$(TARGETDIR_DBG)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDCMD_DBG)

$(TARGETDIR_DBG)/packet.o: source/packet/packet.cpp
	$(BUILDCMD_DBG)

$(TARGETDIR)/main.o: main.cpp
	$(BUILDCMD)

$(TARGETDIR)/socket.o: source/socket/socket.cpp
	$(BUILDCMD)

$(TARGETDIR)/connection.o: source/connection/connection.cpp
	$(BUILDCMD)

$(TARGETDIR)/httpParser.o: source/httpParser/httpParser.cpp
	$(BUILDCMD)

$(TARGETDIR)/packet.o: source/packet/packet.cpp
	$(BUILDCMD)