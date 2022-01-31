#include <string>
#include <json/json.h>
#include <msgpack.hpp>
#include <hiredis/hiredis.h>

using namespace std;

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

    void emit(std::map<string, string> packet);

public:
    Emitter();
    ~Emitter();
    Emitter(const string hostname, const int port);
    Emitter* In(const string channel, const string payload);
    void Emit(const string event, const string data);
    void To(const string channel, const string payload);

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
