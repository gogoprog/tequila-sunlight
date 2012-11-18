#include <boost/asio.hpp>
using namespace boost;

class ComPort
{
public:
    ComPort(std::string port, unsigned int baud_rate)
		:
		io(),
		serial(io,port)
    {
        serial.set_option( boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    void write(const std::string & s)
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    std::string read()
    {
        char
			c;
        std::string
			result;

		result.reserve( 12 );

        for(;;)
        {
            asio::read(serial,asio::buffer(&c,1));

            switch(c)
            {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result+=c;
            }
        }
    }

private:
    boost::asio::io_service
		io;
    boost::asio::serial_port
		serial;
};