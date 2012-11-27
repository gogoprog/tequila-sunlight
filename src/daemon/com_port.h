#include <boost/asio.hpp>
#include <string>
#include <sstream>

using namespace std;
using namespace boost;

class ComPort
{
public:

    ComPort()
        :
        io(),
        serial(io)
    {
    }

    bool isOpen() const
    {
        return serial.is_open();
    }

    void open(const int port_number)
    {
        stringstream
            str;

        str << "/dev/ttyS" << port_number;

        serial.open(str.str());
        serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
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