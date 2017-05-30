#include "ipc/service.hpp"
#include "ipc/service_configuration.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace nepomuk;

int main(int argc, char **argv) try
{
    std::string path = "data/example";

    if (argc > 1)
        path = argv[1];

    if (path.back() == '/')
        path.pop_back();

    auto socket = argc > 2 ? argv[2] : "ipc:///tmp/nepomuk";

    ipc::ServiceConfiguration configuration("ipc.ini", path);
    ipc::Service ipc_service(configuration);

    std::cout << "Listening on: " << socket << std::endl;
    ipc_service.listen(socket);
    std::cout << "Done listening, shutting down." << std::endl;

    return EXIT_SUCCESS;
}
catch (const std::exception &e)
{
    std::cerr << "[error] " << e.what();
    return EXIT_FAILURE;
}
