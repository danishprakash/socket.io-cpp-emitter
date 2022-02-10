#include "emitter.h"
#include <iostream>

using namespace std;

const string UID = "emitter";
const int EVENT = 2;
const int BINARY_EVENT = 2;

Emitter::Emitter()
/* m_hostname(hostname), */
/* m_port(port) */
{
    string hostname = "localhost";
    int port = 7777;
    this->setup(hostname, port);
}

/* void disconnect() */
/* { */
/* } */

/* bool isConnected() */
/* { */
/*     return 1; */
/* } */

Emitter::~Emitter()
{
    // TODO: implement both of these methods
    /* if(isConnected()) */
    /*     this->disconnect(); */
}

void Emitter::setup(const string hostname, const int port)
{
    key = "socket.io";

    nsp = "/";
    broadcast_channel = key + "#" + nsp + "#";
    request_channel = key + "-request#" + nsp + "#";

    this->connectTo(hostname, port);
}

Emitter* Emitter::In(const string channel)
{
    if (rooms.size() == 0)
    {
        rooms.push_back(channel);
    }
    return this;
}

Emitter* Emitter::To(const string channel)
{
    return this->In(channel);
}

void Emitter::Emit(const string event, string data)
{
    vector <string> d;
    d.push_back(event);
    d.push_back(data);

    Packet packet = {
        EVENT,
        d
    };

    this->emit(packet);
}

void Emitter::emit(Packet packet)
{
    cout<<rooms.at(0);
    Opts opts = { rooms };

    // setup msgpack packet object
    tuple<string, Packet, Opts> pack(UID, packet, opts);

    stringstream buffer;
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
    auto reply = (redisReply*)redisCommand(m_connection, "PUBLISH %s %s", channel.c_str(), buffer.c_str());
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
