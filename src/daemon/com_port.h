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

    void open(const string port_string)
    {
        try
        {
            if(serial.is_open())
            {
                serial.close();
            }

            serial.open(port_string);
            serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
        }
        catch(boost::system::system_error)
        {
        }
    }

    ComPort & operator<<(const int code)
    {
        write(code);
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        return * this;
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
    void write(const std::string & s)
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    void write(const int code)
    {
        std::string str;
        str = code;
        boost::asio::write(serial,boost::asio::buffer(str.c_str(),str.size()));
    }

    boost::asio::io_service
        io;
    boost::asio::serial_port
        serial;
};