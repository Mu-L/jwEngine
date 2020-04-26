#include <string>
#include "sol/sol.hpp"

#include "KcpEvent.h"
#include "KcpClient.h"
#include "KcpServer.h"
#include "UdpPacket.h"
#include "KcpSession.h"
#include "EventLoop.h"
#include "PacketPool.h"

class Lua_KcpServer : public KcpServer, public KcpEvent
{
public:
	Lua_KcpServer() :
		KcpServer(EventLoop::Instance(), this)
	{

	}

protected:


	virtual void onAccept(KcpSession * conn) {
		on_accept(conn);
	};
	virtual void onClose(KcpSession * conn) {
		on_close(conn);
	};
	virtual void onMsg(KcpSession * conn, UdpPacket * pack) {
		on_msg(conn, pack);
	};

public:
	std::function<void(KcpSession *)> on_accept;
	std::function<void(KcpSession *)> on_close;
	std::function<void(KcpSession *, UdpPacket *)> on_msg;
};

class Lua_KcpClient : public KcpClient, public KcpEvent
{
public:
	Lua_KcpClient() :
		KcpClient(EventLoop::Instance(), this)
	{

	}

protected:

	virtual void onConnect(KcpSession * conn, int argv) {
		on_connect(conn, argv);
	};
	virtual void onClose(KcpSession * conn) {
		on_close(conn);
	};
	virtual void onMsg(KcpSession * conn, UdpPacket * pack) {
		on_msg(conn, pack);
	};

public:
	std::function<void(KcpSession *, int)> on_connect;
	std::function<void(KcpSession *)> on_close;
	std::function<void(KcpSession *, UdpPacket *)> on_msg;
};

void init_kcppacket_pool(int count)
{
	init_packet_pool<UdpPacket>(count);
}


void luabind_kcpserver(sol::state & lua)
{
	lua["init_kcppacket_pool"] = &init_kcppacket_pool;

	lua.new_usertype<KcpSession>("KcpSession",
		"sendPacket", &KcpSession::sendPacket,
		"sendData", &KcpSession::sendData);

	lua.new_usertype<Lua_KcpClient>("KcpClient",
		//sol::constructors<Lua_NetClient(EventLoop *)>(),
		"createSession", &Lua_KcpClient::createSession,
		"closeSession", &Lua_KcpClient::closeSession,
		"on_connect", &Lua_KcpClient::on_connect,
		"on_close", &Lua_KcpClient::on_close,
		"on_msg", &Lua_KcpClient::on_msg);

	lua.new_usertype<Lua_KcpServer>("KcpServer",
		//sol::constructors<Lua_NetServer(EventLoop *)>(),
		"start", &Lua_KcpServer::start,
		"closeSession", &Lua_KcpServer::closeSession,
		"shutdown", &Lua_KcpServer::shutdown,
		"on_accept", &Lua_KcpServer::on_accept,
		"on_close", &Lua_KcpServer::on_close,
		"on_msg", &Lua_KcpServer::on_msg);

	lua.new_usertype<UdpPacket>("KcpPacket",
		sol::base_classes, sol::bases<BasePacket>());
}