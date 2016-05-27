#pragma once

//------------------------------------------------------------------------
//
//  Name:	GameClient.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include "GameClient.h"

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/format.hpp>

#include "../../ParameterLoader.h"

class GameClientManager
{
public:
	static GameClientManager& getInstance()
	{
		static GameClientManager instance;
		return instance;
	}

	~GameClientManager()
	{
		// #socket�� close�� ȣ���� ����� �ʿ��ϴ�. 
		// _game_client->close();
	
	}

	void send(const GameMessage& msg)
	{
		_game_client->send(msg);
	}

	const std::unique_ptr<GameClient>& getGameClient() const
	{
		return _game_client;
	}

private:
	GameClientManager(const GameClientManager&) = delete; // no copies
	GameClientManager& operator=(const GameClientManager&) = delete; // no self-assignments
	GameClientManager()
	{
		try
		{
			// �б� ��ǻ��(����)
			// std::string name = "220.67.128.17";
			// std::string port = "8001";

			// MIT WIFI�� ����� �� �� ��ǻ��
			// std::string name = "192.168.0.49";
			// std::string port = "8001";

			// MIT WIFI iptime�� ����� �� �� ��ǻ��
			// std::string name = "192.168.0.2";
			// std::string port = "8001";

			// MIT WIFI iptime2�� ����� �� �� ��ǻ��
			// std::string name = "10.10.0.19";
			// std::string port = "8001";

			// Hufs Global�� ����� �� �� ��ǻ��
			// std::string name = "10.10.0.2";
			// std::string port = "8001";

			// ����� WIFI
			// std::string name = "112.108.39.206";
			// std::string port = "8001";

			// std::string name = "112.108.39.151";
			// std::string port = "8001";

			// �� WIFI
			// std::string name = "192.168.0.4";
			// std::string port = "8001";

			std::string name = Prm.IP;
			std::string port = "8001";

			boost::asio::ip::tcp::resolver resolver(_io_service);
			boost::asio::ip::tcp::resolver::query query(name, port);
			boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

			_game_client.reset(new GameClient(_io_service, iterator));
			_async_io_thread.reset(
				new std::thread(
					boost::bind(&boost::asio::io_service::run, &_io_service)));
			_async_io_thread->detach();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}

	std::unique_ptr<GameClient> _game_client;
	std::unique_ptr<std::thread> _async_io_thread;
	boost::asio::io_service _io_service;
};

