#pragma once

#ifndef TCPLOGGER_H
#define TCPLOGGER_H

#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

template <typename K, typename V>
class CnHashMap;

class TsdbLogger
{
	boost::asio::io_service& m_Service;
	boost::asio::ip::tcp::socket m_Socket;
	std::string m_LogServer;
	uint16 m_Port;
public:
	typedef CnHashMap<std::string, std::string> TagMap;
	TsdbLogger(boost::asio::io_service& ioService, std::string logserver, uint16 port);

	void Connect();

	void Log(const std::string& key, uint32 value, const  TagMap& tags = TagMap());
	void Write(Crown::shared_ptr<std::string> message);

private:
	void OnConnected(const boost::system::error_code& error);

	// The shared_ptr<std::string> makes the string existed during the async write.
	void OnWritten(Crown::shared_ptr<std::string>, const boost::system::error_code &error);
	void DoWrite(Crown::shared_ptr<std::string> message);
};
#endif // TCPLOGGER_H
