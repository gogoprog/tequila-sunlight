#include <iostream>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <string>
#include "light_manager.h"
#include "mongoose.h"

using namespace std;
using namespace boost::program_options;
namespace po = boost::program_options;

LightManager
    lightManager;
string
    httpPort("1010");

static void *callback(enum mg_event event, mg_connection *conn) 
{
    const mg_request_info 
        * request_info = mg_get_request_info(conn);

    if (event == MG_NEW_REQUEST)
    {
        string
            command(request_info->uri);

        lightManager.addCommand(command);

        mg_printf(
            conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 0\r\n"
            "\r\n"
            );

        return ( void * )1;
    }
    else
    {
        return NULL;
    }
}

void httpJob()
{
    mg_context 
        * context;
    const char 
        * options[] = { "listening_ports", httpPort.c_str(), NULL };

    context = mg_start(&callback, NULL, options);

    while(1)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        boost::this_thread::yield();
    }
}

void lightJob()
{
    lightManager.initializeComPort();

    while(1)
    {
        lightManager.update(50);
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        boost::this_thread::yield();
    }
}

void help()
{
    cout << "Usage: tequila-sunlight [OPTIONS]" << endl;
    cout << "A daemon that receives http requests and send commands through serial port." << endl;
}

int main(int argc, char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help")
        ("port", po::value<string>(), "Set http port to listen")
        ("comport", po::value<string>(), "Set default serial port")
        ;
    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm); 
    }
    catch(po::error)
    {
        cout << "Command line error." << endl;
        return 2;
    }

    if (vm.count("help"))
    {
        help();
        cout << desc << endl;
        return 1;
    }

    if (vm.count("port"))
    {
        httpPort = vm["port"].as<string>();
    }

    if (vm.count("comport"))
    {
        lightManager.setComPort(vm["comport"].as<string>());
    }


    boost::thread
        http_job_thread(httpJob),
        light_job_thread(lightJob);

    boost::this_thread::yield();

    http_job_thread.join();
    light_job_thread.join();

    return 0;
}
