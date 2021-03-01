#include <stdio.h>
#include <stdlib.h>


#include "NetEvent.h"
#include "EventLoop.h"
#include "NetServer.h"
#include "NetConnect.h"
#include "NetPacket.h"

#include "testmsg.h"

class INetEvent : public NetEvent
{
public:
	virtual void onAccept(NetConnect * conn) {
		if (!conn)
			return;

		testmsg msg;
		msg.id = 1001;
		msg.play.name = "jw";
		msg.play.level = 10;
		msg.play.msgcount = 3;
		msg.array = {110,120,130};

		Attr attr1;
		attr1.attack = 88;
		attr1.hp = 99;

		Attr attr2;
		attr2.attack = 90;
		attr2.hp = 100;

		msg.attrs = { attr1, attr2 };
		msg.vstr = { "aa", "bbb", "ccc" };

		NetPacket packet;
		msg.write(&packet);

		conn->sendMsg(1, &packet);
	};

	virtual void onClose(NetConnect * conn) {

	};

	virtual void onMsg(NetConnect * conn,int msgtype, NetPacket * pack) {
		testmsg msg;
		msg.read(pack);

		printf("id:%d\n", msg.id);
		printf("play.name :%s\n", msg.play.name.c_str());
		printf("play.level:%d\n", msg.play.level);
		printf("play.msgcount:%d\n", msg.play.msgcount);
		for (int i = 0; i < msg.array.size(); ++i)
		{
			printf("array: %d\n", msg.array[i]);
		}

		for (int i = 0; i < msg.attrs.size(); ++i)
		{
			printf("%d attack:%d\n", i, msg.attrs[i].attack);
			printf("%d hp:%d\n", i, msg.attrs[i].hp);
		}

		for (int i = 0; i < msg.vstr.size(); ++i)
		{
			printf("vstr: %s\n", msg.vstr[i].c_str());
		}

		printf("----------------------------------------\n");
	}
};

//#define CLIENT_TEST

int main()
{

	//��ʼ���¼�ѭ��
	INetEvent eve;
	EventLoop::Instance()->init();

	NetServer server(EventLoop::Instance(), &eve);
	server.listen("127.0.0.1", 3001);

	//�����¼�ѭ��
	EventLoop::Instance()->run();

	system("pause");
	return 0;
}