#include "KcpClient.h"
#include "KcpServer.h"
#include "EventLoop.h"
#include "KcpEvent.h"
#include "KcpSession.h"
#include "UdpPacket.h"
#include <time.h>

class KNetEvent : public KcpEvent
{
public:
	virtual KcpSession * createConnect()
	{
		return new KcpSession(this, 1000);
	}

	virtual void onAccept(KcpSession * conn){
		if (conn)
		{
			// ֪ͨ�������пͻ�������
		}
	};

	virtual void onConnect(KcpSession * conn){
		if (conn)
		{
			// ֪ͨ�ͻ������ӳɹ�
			int x = rand() % 100000;
			std::string s(std::to_string(x));
			conn->sendMsg(1, s.c_str(), s.length());
		}
	};
	virtual void onClose(KcpSession * conn){

	};

	virtual void onMsg(KcpSession * conn, int msgtype, UdpPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("%s\n", s.c_str());
		conn->sendMsg(1, pack);
	}

	virtual void onUdpTimeout(KcpSession * s)
	{
		printf("timeout ..............\n");
		server->closeSession(s);
	}

	KcpServer * server;
};

//#define UDP_CLIENT_TEST

int main()
{
	srand((int)time(0));
	EventLoop::Instance()->init();

	//��ʼ���¼�ѭ��
	KNetEvent eve;
	EventLoop::Instance()->init();

#ifdef UDP_CLIENT_TEST
	KcpClient client(EventLoop::Instance(), &eve);
	client.createSession("127.0.0.1", 3001);

#else
	KcpServer server(EventLoop::Instance(), &eve);
	server.start("127.0.0.1", 3001);
	eve.server = &server;

#endif

	EventLoop::Instance()->run();

	system("pause");
	return 0;
}