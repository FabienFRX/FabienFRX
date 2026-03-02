#include "CommandParser.hpp"
#include <iostream>
#include <algorithm>

CommandParser::CommandParser() : _running(true) {}

void CommandParser::run() {
    std::string line;
    while (_running && std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "START") cmdStart(ss);
        else if (command == "TURN") cmdTurn(ss);
        else if (command == "BEGIN") cmdBegin();
        else if (command == "BOARD") cmdBoard();
        else if (command == "INFO") cmdInfo(ss);
        else if (command == "END") cmdEnd();
        else if (command == "ABOUT") cmdAbout();
        else {
            std::cout << "UNKNOWN message ignored" << std::endl;
        }
    }
}

std::pair<int, int> CommandParser::parseCoordinates(std::string &coordStr) {
    std::replace(coordStr.begin(), coordStr.end(), ',', ' ');
    std::stringstream ss(coordStr);
    int x = -1, y = -1;
    ss >> x >> y;
    return {x, y};
}

void CommandParser::doMove() {
    std::pair<int, int> move = _engine.computeMove();
    if (move.first!= -1) {
        _engine.updateCell(move.first, move.second, 1); // 1 = Moi
        std::cout << move.first << "," << move.second << std::endl;
    } else {
        std::cout << "ERROR No moves left" << std::endl;
    }
}

void CommandParser::cmdStart(std::stringstream &ss) {
    int size;
    ss >> size;
    if (size > 0) {
        _engine.start(size);
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "ERROR unsupported size" << std::endl;
    }
}

void CommandParser::cmdTurn(std::stringstream &ss) {
    std::string coords;
    ss >> coords;
    std::pair<int, int> pos = parseCoordinates(coords);

    _engine.updateCell(pos.first, pos.second, 2);
    
    doMove();
}

void CommandParser::cmdBegin() {
    doMove();
}

void CommandParser::cmdBoard() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line == "DONE") break;

        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        int x, y, who;
        if (ss >> x >> y >> who) {
            _engine.updateCell(x, y, who);
        }
    }
    doMove();
}

void CommandParser::cmdInfo(std::stringstream &ss) {
    std::string key, value;
    ss >> key >> value;
    
}

void CommandParser::cmdEnd() {
    _running = false;
}

void CommandParser::cmdAbout() {
    std::cout << "name=\"PbrainGomoku\", version=\"1.0\", author=\"Epitech\", country=\"FR\"" << std::endl;
}