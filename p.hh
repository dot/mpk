#include <msgpack.hpp>
#include <string>

struct Arg {
public:
    std::string name;
    Arg() {}
    Arg(std::string n): name(n) {}
    MSGPACK_DEFINE(name);
};


struct Result {
public:
    int count;
    Result() {}
    MSGPACK_DEFINE(count);
};
