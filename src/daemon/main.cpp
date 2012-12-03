#include <iostream>
#include <boost/thread.hpp>
#include <cstdlib>
#include <string>
#include "light_manager.h"
#include "mongoose.h"

using namespace std;

LightManager
    lightManager;

static void *callback(enum mg_event event, mg_connection *conn) 
{
    const mg_request_info 
        * request_info = mg_get_request_info(conn);

    if (event == MG_NEW_REQUEST)
    {
        string
            command(request_info->uri);

        lightManager.addCommand(command);

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
        * options[] = { "listening_ports", "8080", NULL };

    context = mg_start(&callback, NULL, options);

    while(1)
    {
        boost::this_thread::yield();
    }
}

void lightJob()
{
    while(1)
    {
        lightManager.update(50);
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        boost::this_thread::yield();
    }
}

int main()
{
    boost::thread
        http_job_thread(httpJob),
        light_job_thread(lightJob);

    cout << "TequilaSunlight starting..." << endl;

    boost::this_thread::yield();

    http_job_thread.join();
    light_job_thread.join();
}