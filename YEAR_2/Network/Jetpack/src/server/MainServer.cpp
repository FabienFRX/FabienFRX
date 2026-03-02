/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Apr, 2025                                                     *
 * Title           - B-NWP-400-TLS-4-1-jetpack-fabien.fraixanet                         *
 * Description     -                                                                    *
 *     MainServer                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Server.hpp"

struct param_handling {
    int _port;
    std::vector<std::string> _map;
};

void check_port(param_handling &struc, char *flag, char *port)
{
    std::string strFlag = flag;

    if (strFlag != "-p")
        throw ServerError("Error: wrong flag for port");

    for (int i = 0; port[i] != '\0'; i++) {
        if (port[i] > 57 || port[i] < 48) {
            throw ServerError("Error: wrong port");
        }
    }
    struc._port = atoi(port);
    return;
}

void check_map(param_handling &struc, char *flag, char *map_path)
{
    std::string strFlag = flag;

    if (strFlag != "-m")
        throw ServerError("Error: wrong flag for map");

    std::string path = map_path;
    std::ifstream stream(path);
    if (!stream)
        throw ServerError("Error: with map");

    std::string line;
    while (getline(stream, line)) {
        struc._map.push_back(line);
    }
}

void handling_param(param_handling &struc, int ac, char **av)
{
    if (ac == 4) {
        check_port(struc, av[1], av[2]);
        check_map(struc, av[3], av[4]);
    } else if (ac == 5) {
        check_port(struc, av[1], av[2]);
        check_map(struc, av[3], av[4]);
    } else {
        throw ServerError("USAGE: ./jetpack_server -p <port> -m <map> [-d]");
    }
}

int main(int ac, char **av)
{
    param_handling param;
    try {
        handling_param(param, ac, av);
    } catch (const ServerError &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
    try {
        Server serv(param._port, param._map);
        std::cout << "launch server:" << std::endl;
        serv.launch_server();
    } catch (const ServerError &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
    return 0;
}
