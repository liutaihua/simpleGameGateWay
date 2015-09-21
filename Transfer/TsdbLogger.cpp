#include "TransferPCH.h"
#include <sdu.h>
#include <ctime>
#include <string>

#include "TsdbLogger.h"

using namespace boost::asio::ip;

TsdbLogger::TsdbLogger(boost::asio::io_service& ioService, std::string logserver, uint16 port) :
	m_Service(ioService),
	m_Socket(ioService),
	m_LogServer(logserver),
	m_Port(port)
{
	Connect();
}

void TsdbLogger::Connect()
{
	tcp::resolver resolver(m_Service);
	tcp::resolver::query query(m_LogServer, Crown::ToString(m_Port));
	tcp::resolver::iterator iterator = resolver.resolve(query);

	boost::asio::async_connect(m_Socket, iterator,
	    boost::bind(&TsdbLogger::OnConnected, this,
			boost::asio::placeholders::error));
}

void TsdbLogger::Log(const std::string& key, uint32 value, const TagMap& tags)
{
	std::ostringstream os;
	os << "put " << key << ' ' << time(nullptr) << ' ' << value;

	for (auto itor = tags.begin(); itor != tags.end(); ++itor)
	{
		os << ' ' << itor->first << '=' << itor->second;
	}
	os << '\n'; // Not std::endl or it may go wrong in Windows.

	Crown::shared_ptr<std::string> logStr = Crown::make_shared<std::string>(os.str());

    // TODO: 下面做法是错误的.
	m_Service.post(boost::bind(&TsdbLogger::Write, this, logStr));
}

void TsdbLogger::Write(Crown::shared_ptr<std::string> message)
{
	boost::asio::async_write(m_Socket, boost::asio::buffer(*message),
				 boost::bind(&TsdbLogger::DoWrite, this, message));
}

void TsdbLogger::OnConnected(const boost::system::error_code &error)
{
	if (error)
	{
		m_Socket.close();
		WARN("Cannot connect to %s:%d . Retrying ...", m_LogServer, m_Port);
		Connect();
	}
	else
	{
		INFO("Connected to %s:%d .", m_LogServer, m_Port);
	}
}

void TsdbLogger::OnWritten(Crown::shared_ptr<std::string>, const boost::system::error_code &error)
{
	if (error)
	{
		m_Socket.close();
		WARN("Lost connect to %s:%d . Retrying ...", m_LogServer, m_Port);
		Connect();
	}
}

void TsdbLogger::DoWrite(Crown::shared_ptr<std::string> message)
{
	std::cout << "Sending... " << message << std::endl;

	
	boost::asio::async_write(m_Socket,
		  boost::asio::buffer(*message),
		  boost::bind(&TsdbLogger::OnWritten, this, message,
		    boost::asio::placeholders::error));
}
