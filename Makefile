OPTIMIZE ?= -O0
COMMON_CXXFLAGS=-g $(OPTIMIZE) -Wall -fPIC
CXXFLAGS=$(COMMON_CXXFLAGS)
CXXFLAGS+= -I/usr/local/include/
LDFLAGS+= -L/usr/local/lib/ -lPocoFoundation -lPocoUtil -lPocoNet -lmsgpack

.PHONY: all
all: server client

server:p.hh server.cc
	g++ $(CXXFLAGS) $(LDFLAGS) server.cc -o $@
client:p.hh client.cc
	g++ $(CXXFLAGS) $(LDFLAGS) client.cc -o $@

.PHONY: clean
clean:
	rm -rf server client
