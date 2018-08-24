#ifndef SESSION_HPP
#define SESSION_HPP

#include "message.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

class Session
{
	public:
		Session(boost::asio::io_context &io_context, tcp::socket socket)
			: io_context_(io_context),
			socket_(std::move(socket))
			{
				// Session starts
				read_header_();
			}

		void write(const Message &msg)
		{
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push(msg);
			if (!write_in_progress)
				write_();
		}

		void run()
		{
			std::thread t([this](){ io_context_.run(); });

			std::string line;
			while(std::getline(std::cin, line))
			{
				std::cout << "Local: " << line << '\n';
				Message msg;
				msg.body_size(line.size());
				std::copy(line.begin(), line.end(), msg.body());
				msg.encode_header();
				write(msg);
			}

			t.join();
		}

	private:
		boost::asio::io_context &io_context_;
		tcp::socket socket_;
		Message read_msg_;
		std::queue<Message> write_msgs_;

		void read_header_()
		{
			boost::asio::async_read(socket_,
					boost::asio::buffer(read_msg_.data(), Message::header_size),
					[this](boost::system::error_code ec, std::size_t /*size*/)
					{
						if (!ec)
						{
							read_msg_.decode_header();
							read_body_();
						}
						else
						{
							std::cout << "Connection aborted: " << ec.message() << '\n';
							socket_.close();
						}
					});
		}

		void read_body_()
		{
			boost::asio::async_read(socket_,
					boost::asio::buffer(read_msg_.body(), read_msg_.body_size()),
					[this](const boost::system::error_code &ec, std::size_t /*size*/)
					{
						if (!ec)
						{
							std::cout << "Remote: ";
							std::cout.write(read_msg_.body(), read_msg_.body_size());
							std::cout << '\n';
							read_header_();
						}
						else
						{
							std::cout << "Connection aborted: " << ec.message() << '\n';
							socket_.close();
						}
					});
		}

		void write_()
		{
			boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front().data(),
						write_msgs_.front().size()),
					[this](const boost::system::error_code &ec, std::size_t /*size*/)
					{
						if (!ec)
						{
							write_msgs_.pop();
							if (!write_msgs_.empty())
								write_();
						}
						else
						{
							std::cout << "Connection aborted: " << ec.message() << '\n';
							socket_.close();
						}
					});
		}
};

#endif
