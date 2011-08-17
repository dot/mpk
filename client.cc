#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetException.h>
#include <Poco/Exception.h>
#include <iostream>
#include "p.hh"

using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Net::Socket;

int main(int argc, char** argv) {
    if ( argc != 2 ) {
        std::cout << "Usage: " << argv[0] << " [message]" << std::endl;
        exit(1);
    }
    std::cout << "starting..." <<  std::endl;
    SocketAddress addr("localhost", 8888);
    StreamSocket socket;
    try {
        std::cout << "connecting..." <<  std::endl;
        socket.connect(addr);

        Arg a(argv[1]);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, a);

        std::cout << "sending..." <<  std::endl;
        size_t length = sbuf.size();
        socket.sendBytes(&length, sizeof(length));
        socket.sendBytes(sbuf.data(), length);
        std::cout << "done" <<  std::endl;

        // receive result
        std::cout << "recieving result..." <<  std::endl;
        socket.receiveBytes(&length, sizeof(length));
        char* data = (char*)malloc(length);
        socket.receiveBytes(data, length);
        msgpack::unpacked msg;
        msgpack::unpack(&msg, data, length);
        msgpack::object obj = msg.get();

        Result r;
        obj.convert(&r);
        std::cout << "length: " << r.count << std::endl;

    } catch (Poco::Exception& exc) {
        std::cerr << "failed: " << exc.displayText() << std::endl;
    }

    socket.close();
    return 0;
}
