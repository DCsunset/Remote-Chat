#include "session.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <memory>

using boost::asio::ip::tcp;

class Server
{
	public:
		std::unique_ptr<Session> session;

		Server(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
			: io_context_(io_context),
			acceptor_(io_context, endpoint)
			{
				accept_();
			}

	private:
		boost::asio::io_context &io_context_;
		tcp::acceptor acceptor_;

		void accept_()
		{
			tcp::socket socket(io_context_);
			// Sync accept
			acceptor_.accept(socket);
			std::cout << "Connected to " << socket.remote_endpoint() << '\n';
			session = std::make_unique<Session>(io_context_, std::move(socket));
		}
};
