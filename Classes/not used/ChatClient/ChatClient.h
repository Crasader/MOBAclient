#pragma once

//------------------------------------------------------------------------
//
//  Name:	ChatClient.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <deque>
#include <queue>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include "ChatMessage.hpp"

#include "ui/UIText.h"

class ChatClient
{
public:
	ChatClient(
		boost::asio::io_service& io_service,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	void write(const ChatMessage& msg);

	void close();

	std::queue<std::string>& getReadMsgQ()
	{
		return _read_msg_queue;
	}

private:

	void handleConnect(const boost::system::error_code& error);

	void handleReadHeader(const boost::system::error_code& error);

	void handleReadBody(const boost::system::error_code& error);

	// �񵿱� IO���� ���� �����ؾ� �ϴ� ����,
	// write�� �� ������ ���� ���¿��� write�ؾ� �� buffer�� �ǵ帮�� ���̴�.
	// �� ������ �ذ��ϱ� ���� doWrite�� handleWrite�� ������ ���� ��ġ�� �� �д�.
	// 1. ���� deque�� msg�� �����ִ��� Ȯ���Ѵ�.
	//    msg�� �����ִٴ� ���� ���� write�� �� ������ �ʾҴٴ� �ǹ��̰�, �̸� flag�� üũ�صд�.
	// 2. �� �� doWrite������ deque�� write�� msg�� �ִ´�.
	//    doWrite���� ȣ���� async_write�� ������, ������ �񵿱� IO�� �����ٴ� �ǹ��̴�.
	// 3. �׷��Ƿ� doWrite�� ȣ���ϴ� callback�Լ��� handleWrite������ 
	//    deque�� msg�� ���ش�.
	// 4. deque�� �� msg�� �������� �� �ִ�. 
	//    �׷��Ƿ� �����ִٸ� �ٽ� callback���� handleWrite�� �־��ش�.
	void doWrite(ChatMessage msg);

	void handleWrite(const boost::system::error_code& error);

	void doClose();

private:

	//boost::lockfree::queue<char> chat_queue;

	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::socket _socket;
	ChatMessage _read_msg;
	std::deque<ChatMessage> _write_msgs;
	
	std::queue<std::string> _read_msg_queue;
};