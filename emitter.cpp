#include "emitter.h"
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <grp.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/wait.h>

using namespace std;

const string UID = "emitter";
const int EVENT = 2;
const int BINARY_EVENT = 2;

Emitter::Emitter():
m_hostname(""),
m_port(0)
{
}

Emitter::Emitter(const string hostname, const int port):
m_hostname(hostname),
m_port(port)
{
    this->setup(hostname, port);
}

void disconnect()
{
}

bool isConnected()
{
    return 1;
}

Emitter::~Emitter()
{
    // TODO: implement both of these methods
    if(isConnected())
        this->disconnect();
}

void Emitter::setup(const string hostname, const int port)
{
    key = "socket.io";

    nsp = "/";
    broadcast_channel = key + "#" + nsp + "#";
    request_channel = key + "-request#" + nsp + "#";

    this->connectTo(hostname, port);
}

Emitter* Emitter::In(std::string channel, std::string payload)
{
    return this;
}

void Emitter::To(const string channel, const string payload)
{
    rooms.push_back(channel);
    this->In(channel, payload);
}

void Emitter::Emit(const string event, const string data)
{
    std::map<string, string> packet;
    packet["type"] = EVENT;
    packet["data"] = data;

    this->emit(packet);
}

void Emitter::emit(std::map<string, string> packet)
{
    std:string flag = flags.at("nsp");
    if (flags.find("nsp") != flags.end())
    {
        packet["nsp"] = flags.at("nsp");
    }

    std::map<string, string> opts;
    opts["rooms"] = "";
    opts["flags"] = "";
    opts["except"] = "";

    // setup msgpack packet object
    std::tuple<string, map<string, string>, map<string, string>> pack(UID, packet, opts);

    // TODO: encode msgpack payload
    std::stringstream buffer;
    msgpack::pack(buffer, pack);

    string channel = broadcast_channel;
    if (rooms.size() == 1)
    {
        channel = broadcast_channel + rooms[0] + "#";
    }

    this->publish(channel, buffer.str());
}

void Emitter::publish(string channel, string buffer)
{
    // Publish to redis
    auto reply = (redisReply*)redisCommand(m_connection, "PUBLISH %s %s", channel, buffer);
    if (reply)
        freeReplyObject(reply);
    return;
}

void Emitter::connectTo(const string hostname, const int port)
{
    // Sanity
    if(hostname.empty() || port == 0)
        return;

    // Try a connection
    struct timeval timeout = { 0, 500000 };
    m_connection = redisConnectWithTimeout(hostname.c_str(), port, timeout);
    if (m_connection == NULL || m_connection->err)
    {
        // printf("Unable to set up connection with redis: %s" + m_connection->errstr);
        printf("Unable to set up connection with redis");
        return;
    }
    m_connected = true;
    m_hostname = hostname;
    m_port = port;
}
