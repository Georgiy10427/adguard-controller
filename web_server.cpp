#include "server_http.hpp"
#include <future>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include "installer.hpp"

//#define DEBUG

using namespace std;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

shared_ptr<HttpServer::Response> render_page(string path, 
                shared_ptr<HttpServer::Response> response, 
                SimpleWeb::StatusCode status_code = SimpleWeb::StatusCode::success_ok)
{
    ifstream doc(path);

    if (doc.is_open())
    {
        stringstream stream;
        string line;
        while (getline(doc, line))
        {
            stream << line;
        }
        response->write(status_code, stream);
    }
    else if (path == string("web/404.html"))
    {
        response->write(
            SimpleWeb::StatusCode::server_error_internal_server_error,
            "File \"" + path + "\" doesn't exist.\n" + \
            "The server may be running in the wrong working " + \
            "directory or web directory doesn't exist.");
    } 
    else {
        doc.close();
        return render_page("web/404.html", 
                           response,
                           status_code=SimpleWeb::StatusCode::client_error_not_found);
    }
    doc.close();
    return response;
}

int get_cpu_temp()
{
    string line;
    ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (file.is_open())
    {
        getline(file, line);
        return int(stoi(line)/1000);
        file.close();
    } else {
        file.close();
        return 0;
    }
    return 0;
}

void run_server()
{
    HttpServer server;
    
    #ifdef DEBUG
    server.config.port = 8080;
    #endif

    #ifndef DEBUG
    server.config.port = 80;
    #endif

    // Callback for main page
    server.resource["/"]["GET"] = [](shared_ptr<HttpServer::Response> response, 
                                     shared_ptr<HttpServer::Request> request) {
        render_page("web/index.html", response);
        cout << "root\n";
    };

    // For CSS, JS, ICO, HTML files.
    server.resource[".*.css$|.*.js$|.*.ico|.*.html"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                    shared_ptr<HttpServer::Request> request) {
        render_page("web" + request->path, response);
        cout << request->path << endl;
    };

    // For power off requests.
    server.resource["/power-off"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                    shared_ptr<HttpServer::Request> request) {
        cout << "Power off\n";
        #ifndef DEBUG
        system("shutdown now");
        #endif
    };

    server.resource["/reboot"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                            shared_ptr<HttpServer::Request> request)
    {
        cout << "Reboot\n";
        #ifndef DEBUG
        system("reboot");
        #endif
    };

    server.resource["/config"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                           shared_ptr<HttpServer::Request> request) {
        #ifdef DEBUG
        render_page("web/404.html", response);
        #endif
        #ifndef DEBUG
        render_page("/opt/AdGuardHome/AdGuardHome.yaml", response);
        #endif
        cout << "Config sended.\n";
    };

    server.resource["/update-config"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                   shared_ptr<HttpServer::Request> request)
    {
        ofstream config;
        config.open("/opt/AdGuardHome/AdGuardHome.yaml");
        if (config.is_open())
        {
            #ifndef DEBUG
            system("/opt/AdGuardHome/AdGuardHome -s stop");
            #endif
            config << request->content.string();
            #ifndef DEBUG
            system("/opt/AdGuardHome/AdGuardHome -s start");
            #endif
        } else {
            cout << "Couldn't write config. Body:" << request->content.string() << endl;
        }
        config.close();
    };

    server.resource["/cpu-temp"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                             shared_ptr<HttpServer::Request> request) 
    {
        cout << "CPU temp: " << get_cpu_temp() << endl;
        stringstream stream;
        stream << get_cpu_temp();
        response->write(stream);
    };

    server.on_error = [](shared_ptr<HttpServer::Request>, 
                         const SimpleWeb::error_code & /*ec*/) {
      // Handle errors here
      // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };
    
    // Start server
    promise<unsigned short> server_port;
    thread server_thread([&server, &server_port]() {
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    server_thread.join();
}

int main()
{
    run_installer();
    run_server();
    return 0;
}