/************************************************************************
* @file      WebSocketConnect.h
* @brief     websocketЭ�����
* @author    jiangwang
* @data      2019-12-14
* @version   0.1
************************************************************************/

#pragma once

#include <string>
#include <queue>
#include "TcpServer.h"

class WebSocketEvent;
class WebSocketPacket;
class WebSocketConnect : public TcpSocket
{
public:
	WebSocketConnect(WebSocketEvent * wevent, uint32 buffersize = MESSAGE_BUFFER_SIZE);
	~WebSocketConnect();

	// ���� WebSocket ����������
	bool parseHandshake(const char * pData, int len);

	// �ؿͻ��˵�����Э��
	void respondHandshake(std::string & msg);

	void sendMsg(WebSocketPacket * pack);
	void sendMsg(const char * msg, uint32 len);
	// lua call
	void sendPacket(WebSocketPacket * pack) { sendMsg(pack); }
	void sendData(std::string_view view) { sendMsg(view.data(), view.size()); }

	static WebSocketPacket * createPacket();
	static void recyclePacket(WebSocketPacket * pack);
protected:

	virtual void on_msgbuffer(MessageBuffer * buffer);
	virtual void on_clsesocket();
	virtual void on_writecomplete();

private:
	void send_top_msg();
	bool decodingDatas(WebSocketPacket* pPacket, uint32 msg_mask);
	void release();
private:
	std::string __m_strKey;
	std::string __m_strHost;
	std::string __m_handshakeMsg;
	bool __m_isHandshake;
	WebSocketEvent * __m_webevent;
	WebSocketPacket * __m_readPacket;
	std::queue<WebSocketPacket *> __m_sendPackets;
};

