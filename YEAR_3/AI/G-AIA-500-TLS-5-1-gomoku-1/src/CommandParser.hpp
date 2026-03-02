#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include "GomokuEngine.hpp"
#include <string>
#include <sstream>

class CommandParser {
public:
    CommandParser();
    void run();

private:
    GomokuEngine _engine;
    bool _running;

    void cmdStart(std::stringstream &ss);
    void cmdTurn(std::stringstream &ss);
    void cmdBegin();
    void cmdBoard();
    void cmdInfo(std::stringstream &ss);
    void cmdEnd();
    void cmdAbout();
    
    void doMove();
    std::pair<int, int> parseCoordinates(std::string &coordStr);
};

#endif