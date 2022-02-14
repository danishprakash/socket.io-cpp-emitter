#include <json/json.h>
#include <msgpack.hpp>
#include <hiredis/hiredis.h>

using namespace std;

struct Opts
{
    vector <string> rooms;
    map<std::string, msgpack::type::variant> flags;
    MSGPACK_DEFINE_MAP(rooms, flags);

    /* vector <string> except; */
};


struct Packet
{
    int type;
    vector<string> data;
    MSGPACK_DEFINE_MAP(type, data);
};


class Emitter
{

private:
    // redis options
    string m_hostname;
    int m_port;
    string address;
    redisContext *m_connection;

    // broadcast options
    string key;
    string nsp;
    string broadcast_channel;
    string request_channel;
    bool m_connected;
    vector <string> rooms;
    std::map<string, string> flags;

    void emit(Packet packet);

public:
    Emitter();
    ~Emitter();
    Emitter(string hostname, int port);
    Emitter* In(const string channel);
    Emitter* To(const string channel);
    void Emit(const string event, string data);

    // redis actions
    void connectTo(const string hostname, const size_t port);
    void disconnect();
    bool isConnected();
    void setup(const string hostname, const int port);
    void connectTo(const string hostname, const int port);
    void publish(string channel, string payload);
};
