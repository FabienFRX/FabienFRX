/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** IDirectoryLister
*/

#ifndef IDIRECTORYLISTER_HPP_
    #define IDIRECTORYLISTER_HPP_

    #include <string>

class IDirectoryLister {
public:
    virtual ~IDirectoryLister() = default;

    virtual bool open(const std::string& path, bool hidden) = 0;
    virtual std::string get() = 0;

    class OpenFailureException : public std::exception {
    public:
        explicit OpenFailureException(const std::string& path);
        const char* what() const noexcept override;
    private:
        std::string m_message;
    };

    class NoMoreFileException : public std::exception {
    public:
        const char* what() const noexcept override;
    };
};

#endif /* !IDIRECTORYLISTER_HPP_ */
