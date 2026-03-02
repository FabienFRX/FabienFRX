/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     NcursesDisplay                                                                   *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/NcursesDisplay.hpp"

Krell::NcursesDisplay::NcursesDisplay()
{
    initscr(); // Initialise nCurses
    if (!stdscr) {
        throw std::runtime_error("Failed to initialize nCurses!");
    }
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Make getch non-blocking
}

Krell::NcursesDisplay::~NcursesDisplay()
{
    endwin(); // End nCurses
}

void Krell::NcursesDisplay::draw(const std::string &data)
{
    if (data.empty()) {
        return;
    } else {
        clear(); // Clear the screen before drawing new data
        printw(data.c_str());
        refresh();
    }
}