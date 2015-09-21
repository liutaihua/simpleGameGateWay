#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include "ServerClientSocket.h"

class SClientSocket;

namespace tcpproxy
{
   namespace ip = boost::asio::ip;

   class bridge : public boost::enable_shared_from_this<bridge>
   {
   public:

      typedef ip::tcp::socket socket_type;
      typedef boost::shared_ptr<bridge> ptr_type;

      bridge(boost::asio::io_service& ios, SClientSocket* parent, const std::string& upstream_host, unsigned short upstream_port)
      : downstream_socket_(ios),
        upstream_socket_(ios),

        m_ParentSocket(parent)
      {
          upstream_socket_.async_connect(
               ip::tcp::endpoint(
                    boost::asio::ip::address::from_string(upstream_host),
                    upstream_port),
                boost::bind(&bridge::handle_upstream_connect,
                     shared_from_this(),
                            boost::asio::placeholders::error));
      }

      socket_type& downstream_socket()
      {
         return downstream_socket_;
      }

      socket_type& upstream_socket()
      {
         return upstream_socket_;
      }

      void start(const std::string& upstream_host, unsigned short upstream_port)
      {
         upstream_socket_.async_connect(
              ip::tcp::endpoint(
                   boost::asio::ip::address::from_string(upstream_host),
                   upstream_port),
               boost::bind(&bridge::handle_upstream_connect,
                    shared_from_this(),
                    boost::asio::placeholders::error));
      }

      void handle_upstream_connect(const boost::system::error_code& error)
      {
         if (!error)
         {
            upstream_socket_.async_read_some(
                 boost::asio::buffer(upstream_data_,max_data_length),
                 boost::bind(&bridge::handle_upstream_read,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));

            downstream_socket_.async_read_some(
                 boost::asio::buffer(downstream_data_,max_data_length),
                 boost::bind(&bridge::handle_downstream_read,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
         }
         else
            close();
      }

   private:

      void handle_downstream_write(const boost::system::error_code& error)
      {
         if (!error)
         {
            upstream_socket_.async_read_some(
                 boost::asio::buffer(upstream_data_,max_data_length),
                 boost::bind(&bridge::handle_upstream_read,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
         }
         else
            close();
      }

      void handle_downstream_read(const boost::system::error_code& error,
                                  const size_t& bytes_transferred)
      {
         if (!error)
         {
            async_write(upstream_socket_,
                  boost::asio::buffer(downstream_data_,bytes_transferred),
                  boost::bind(&bridge::handle_upstream_write,
                        shared_from_this(),
                        boost::asio::placeholders::error));
         }
         else
            close();
      }

      void handle_upstream_write(const boost::system::error_code& error)
      {
         if (!error)
         {
            downstream_socket_.async_read_some(
                 boost::asio::buffer(downstream_data_,max_data_length),
                 boost::bind(&bridge::handle_downstream_read,
                      shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
         }
         else
            close();
      }

      void handle_upstream_read(const boost::system::error_code& error,
                                const size_t& bytes_transferred)
      {
         if (!error)
         {
            async_write(downstream_socket_,
                 boost::asio::buffer(upstream_data_,bytes_transferred),
                 boost::bind(&bridge::handle_downstream_write,
                      shared_from_this(),
                      boost::asio::placeholders::error));
         }
         else
            close();
      }

      void close()
      {
         boost::mutex::scoped_lock lock(mutex_);
         if (downstream_socket_.is_open())
            downstream_socket_.close();
         if (upstream_socket_.is_open())
            upstream_socket_.close();
      }

      socket_type downstream_socket_;
      socket_type upstream_socket_;

      enum { max_data_length = 8192 }; //8KB
      unsigned char downstream_data_[max_data_length];
      unsigned char upstream_data_[max_data_length];

      SClientSocket* m_ParentSocket;

      boost::mutex mutex_;

//      class acceptor
//      {
//      public:

//         acceptor(boost::asio::io_service& io_service,
//                  const std::string& local_host, unsigned short local_port,
//                  const std::string& upstream_host, unsigned short upstream_port)
//         : io_service_(io_service),
//           localhost_address(boost::asio::ip::address_v4::from_string(local_host)),
//           acceptor_(io_service_,ip::tcp::endpoint(localhost_address,local_port)),
//           upstream_port_(upstream_port),
//           upstream_host_(upstream_host)
//         {}

//         bool accept_connections()
//         {
//            try
//            {
//               session_ = boost::shared_ptr<bridge>(new bridge(io_service_));
//               acceptor_.async_accept(session_->downstream_socket(),
//                    boost::bind(&acceptor::handle_accept,
//                         this,
//                         boost::asio::placeholders::error));
//            }
//            catch(std::exception& e)
//            {
//               std::cerr << "acceptor exception: " << e.what() << std::endl;
//               return false;
//            }
//            return true;
//         }

//      private:

//         void handle_accept(const boost::system::error_code& error)
//         {
//            if (!error)
//            {
//               session_->start(upstream_host_,upstream_port_);
//               if (!accept_connections())
//               {
//                  std::cerr << "Failure during call to accept." << std::endl;
//               }
//            }
//            else
//            {
//               std::cerr << "Error: " << error.message() << std::endl;
//            }
//         }

//         boost::asio::io_service& io_service_;
//         ip::address_v4 localhost_address;
//         ip::tcp::acceptor acceptor_;
//         ptr_type session_;
//         unsigned short upstream_port_;
//         std::string upstream_host_;
//      };

    };
}
