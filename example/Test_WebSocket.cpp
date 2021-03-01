#include "WebSocketConnect.h"
#include "WebSocketEvent.h"
#include "WebSocketPacket.h"
#include "WebSocketServer.h"
#include "EventLoop.h"
#include "Tools.h"
#include <stdio.h>


class IWebEvent : public WebSocketEvent
{
public:
	virtual void onHandshake(WebSocketConnect * conn){
		printf("���ֳɹ�\n");

	};
	virtual void onAccept(WebSocketConnect * conn){
	
	};

	virtual void onClose(WebSocketConnect * conn){
		printf("close connect\n");
	};

	virtual void onMsg(WebSocketConnect * conn, WebSocketPacket * pack){
		const char * p = pack->getBodyData();

		char buff[1024] = {0};
		strncpy(buff, p, pack->getBodySize());

		std::string s = Tools::utf8ToGbk(buff);
		printf("%s\n", s.c_str());

		conn->sendMsg(pack);
	};
};


int main()
{
	EventLoop::Instance()->init();

	IWebEvent wevent;
	WebSocketServer server(EventLoop::Instance(), &wevent);
	server.listen("127.0.0.1", 8080);

	EventLoop::Instance()->run();
	return 0;
}