/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Apr, 2025                                                     *
 * Title           - B-NWP-400-TLS-4-1-jetpack-fabien.fraixanet                         *
 * Description     -                                                                    *
 *     Error                                                                            *
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

#ifndef ERROR_HPP_
    #define ERROR_HPP_

#include <string>
#include <exception>

class ServerError : std::exception {
    public:
        ServerError(const std::string &msg);
        ~ServerError() = default;

        const char* what() const noexcept override;
        
    private:
        std::string _msg;
};

class ClientError : std::exception {
    public:
        ClientError(const std::string &msg);
        ~ClientError() = default;

        const char* what() const noexcept override;
        
    private:
        std::string _msg;
};

 
 #endif /* !ERROR_HPP_ */