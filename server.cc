#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Exception.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <msgpack.hpp>
#include "p.hh"

using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServer;
using Poco::Util::ServerApplication;
using Poco::Util::Application;

class MsgServerConnection: public TCPServerConnection {
public:
    MsgServerConnection(const StreamSocket& s) : TCPServerConnection(s) {}
    void run() {
        Application& app = Application::instance();
        app.logger().information("Request from " + this->socket().peerAddress().toString());
        try {
            size_t length;
            socket().receiveBytes(&length, sizeof(length));
            char* data = (char*)malloc(length);
            socket().receiveBytes(data, length);
            msgpack::unpacked msg;
            msgpack::unpack(&msg, data, length);
            msgpack::object obj = msg.get();

            Arg a;
            obj.convert(&a);

            app.logger().information("content: " + a.name);

            Result r;
            r.count = strlen(a.name.c_str());
            msgpack::sbuffer sbuf;
            msgpack::pack(sbuf, r);

            length = sbuf.size();
            socket().sendBytes(&length, sizeof(length));
            socket().sendBytes(sbuf.data(), length);

            // do something
        } catch (Poco::Exception& exc) {
            app.logger().log(exc);
        }
    }
};

class MsgServer: public Poco::Util::ServerApplication {
public:
    MsgServer() {}
    ~MsgServer() {}
protected:
    void initialize(Application& self) {
        ServerApplication::initialize(self);
    }

    void uninitialize() {
        ServerApplication::uninitialize();
    }

    int main(const std::vector<std::string>& args) {
        unsigned short port = 8888;
        ServerSocket svs(port);
        TCPServer srv(new TCPServerConnectionFactoryImpl<MsgServerConnection>(), svs);
        srv.start();
        waitForTerminationRequest();
        srv.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    MsgServer app;
    return app.run(argc, argv);
}
