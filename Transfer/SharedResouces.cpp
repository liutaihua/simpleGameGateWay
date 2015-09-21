#include "TransferPCH.h"
#include "SharedResouces.h"


SharedResouces::SharedResouces()
{
}

boost::asio::io_service &SharedResouces::IOService()
{
	static boost::asio::io_service service;
	return service;
}
