#include "client.hpp"
#include "server.hpp"
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <string>

constexpr char version[] = "v0.1.0";

int main(int argc, char *argv[])
{
	unsigned short port = 4518;
	std::string address;
	boost::asio::io_context io_context;

	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "Print help message")
		("version,v", "Print version info")
		("port,p", po::value<unsigned short>(&port), "Port to connect or listen (default: 4518)")
		("listen,l", "Listen to accept connections")
		("address,a", po::value<std::string>(&address), "Peer IP address"); 
	po::variables_map vm;

	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch(po::error &e)
	{
		std::cerr << e.what() << "\n\n";
		std::cerr << "Use --help to see usage\n";
		return 1;
	}

	if(vm.count("help"))
	{
		std::cout << "\nUsage: encrypt [options]\n\n";
		std::cout << desc << '\n';
		return 0;
	}
	if(vm.count("version"))
	{
		std::cout << "Remote Chat " << version << '\n'
			<< "By DCsunset\n\n";
		return 0;
	}
	if(!vm.count("address"))
	{
		std::cerr << "Listening address required\n";
		return 1;
	}
	try
	{
		tcp::endpoint endpoint(boost::asio::ip::make_address(address), port);
		if(vm.count("listen"))
		{
			std::cout << "Listening on " << endpoint << '\n';
			Server server(io_context, endpoint);
			server.session->run();
		}
		else
		{
			std::cout << "Connecting to " << endpoint << '\n';
			Client client(io_context, endpoint);
			client.session->run();
		}
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
