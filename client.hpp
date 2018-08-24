#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "session.hpp"
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Client
{
	public:
		std::unique_ptr<Session> session;

		Client(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
			: io_context_(io_context)
		{
			connect_(endpoint);
		}

	private:
		boost::asio::io_context &io_context_;

		void connect_(const tcp::endpoint &endpoint)
		{
			tcp::socket socket(io_context_);
			socket.connect(endpoint);
			std::cout << "Connected to " << endpoint << '\n';
			session = std::make_unique<Session>(io_context_, std::move(socket));
		}
};

#endif
