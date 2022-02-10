#include <json/json.h>
#include <msgpack.hpp>
#include <hiredis/hiredis.h>

using namespace std;

struct Opts
{
    vector <string> rooms;
    /* vector <string> except; */
    map<std::string, msgpack::type::variant> flags;
    MSGPACK_DEFINE_MAP(rooms, flags);
};


struct Packet
{
    int type;
    // string nsp;
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

    // broadcast options
    string key;
    string nsp;
    string broadcast_channel;
    string request_channel;
    bool m_connected;
    vector <string> rooms;
    std::map<string, string> flags;
    redisContext *m_connection;


    // emitter options
    string protocol;

    void emit(Packet packet);

public:
    Emitter();
    ~Emitter();
    // Emitter(string hostname, int port);
    Emitter* In(const string channel);
    Emitter* To(const string channel);
    void Emit(const string event, string data);

    /* RedisProvider(); */
    /* RedisProvider(const string hostname, const size_t port); */
    /* ~RedisProvider(); */

    // Connect actions
    void connectTo(const string hostname, const size_t port);
    void disconnect();
    bool isConnected();
    void setup(const string hostname, const int port);
    void connectTo(const string hostname, const int port);

    // Publish
    void publish(string channel, string payload);
    /* void SET_EXPIRE(string key, int seconds); */
    /* bool JSON_SET(string key, string path, string payload); */
    /* bool JSON_ARRINSERT(string key, string path, int offset, string value); */
    /* bool JSON_NUMINCRBY(string key, string path, int value); */
};
