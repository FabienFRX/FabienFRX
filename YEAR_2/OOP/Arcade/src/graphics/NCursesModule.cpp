/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     NCursesModule                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "NCursesModule.hpp"
 
namespace arcade {
    NCursesModule::NCursesModule() : ADisplayModule("NCurses", 0, 0), mouseSupport(false) {}

    NCursesModule::~NCursesModule()
    {
        stop();
    }

    void NCursesModule::init()
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        curs_set(0);

        if (has_colors()) {
        start_color();
            initColors();
        }

        mouseSupport = initMouseSupport();

        getmaxyx(stdscr, height, width);
        open = true;
    }

    bool NCursesModule::initMouseSupport()
    {
        if (!has_mouse()) {
            std::cerr << "Warning: Terminal does not appear to have mouse support" << std::endl;
        }

        mmask_t availableMask;
        mousemask(ALL_MOUSE_EVENTS, &availableMask);
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);
        
        return true;
    }

    void NCursesModule::stop()
    {
        if (open) {
            if (mouseSupport) {
                mousemask(0, NULL);
            }
            clear();
            refresh();
            endwin();
            open = false;
        }
    }

    void NCursesModule::initColors()
    {
        init_pair(1, COLOR_BLACK, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_BLUE, COLOR_BLACK);
        init_pair(6, COLOR_YELLOW, COLOR_BLACK);
        init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(8, COLOR_CYAN, COLOR_BLACK);

        colorPairs[COLOR_BLACK] = 1;
        colorPairs[COLOR_WHITE] = 2;
        colorPairs[COLOR_RED] = 3;
        colorPairs[COLOR_GREEN] = 4;
        colorPairs[COLOR_BLUE] = 5;
        colorPairs[COLOR_YELLOW] = 6;
        colorPairs[COLOR_MAGENTA] = 7;
        colorPairs[COLOR_CYAN] = 8;
    }

    int NCursesModule::getColorPair(const Color &color)
    {
        if (color.r > 200 && color.g > 200 && color.b > 200) {
            return COLOR_PAIR(colorPairs[COLOR_WHITE]);
        } else if (color.r > 200 && color.g < 100 && color.b < 100) {
            return COLOR_PAIR(colorPairs[COLOR_RED]);
        } else if (color.r < 100 && color.g > 200 && color.b < 100) {
            return COLOR_PAIR(colorPairs[COLOR_GREEN]);
        } else if (color.r < 100 && color.g < 100 && color.b > 200) {
            return COLOR_PAIR(colorPairs[COLOR_BLUE]);
        } else if (color.r > 200 && color.g > 200 && color.b < 100) {
            return COLOR_PAIR(colorPairs[COLOR_YELLOW]);
        } else if (color.r > 200 && color.g < 100 && color.b > 200) {
            return COLOR_PAIR(colorPairs[COLOR_MAGENTA]);
        } else if (color.r < 100 && color.g > 200 && color.b > 200) {
            return COLOR_PAIR(colorPairs[COLOR_CYAN]);
        } else {
            return COLOR_PAIR(colorPairs[COLOR_BLACK]);
        }
    }

    void NCursesModule::clear()
    {
        ::clear();
    }

    void NCursesModule::display()
    {
        refresh();
        doupdate();
    }

    bool NCursesModule::pollEvent(InputEvent &event)
    {
        int ch = getch();

        if (ch == KEY_MOUSE) {
            MEVENT mouseEvent;
            if (getmouse(&mouseEvent) == OK) {
                if (mouseEvent.bstate & BUTTON1_CLICKED) {
                    event.type = EventType::MouseButtonPressed;
                    event.mouseButton.x = mouseEvent.x;
                    event.mouseButton.y = mouseEvent.y;
                    event.mouseButton.button = 0;
                    return true;
                }
                else if (mouseEvent.bstate & BUTTON2_CLICKED) {
                    event.type = EventType::MouseButtonPressed;
                    event.mouseButton.x = mouseEvent.x;
                    event.mouseButton.y = mouseEvent.y;
                    event.mouseButton.button = 1;
                    return true;
                }
                else if (mouseEvent.bstate & BUTTON3_CLICKED) {
                    event.type = EventType::MouseButtonPressed;
                    event.mouseButton.x = mouseEvent.x;
                    event.mouseButton.y = mouseEvent.y;
                    event.mouseButton.button = 2;
                    return true;
                }
                else if (mouseEvent.bstate & REPORT_MOUSE_POSITION) {
                    event.type = EventType::MouseMoved;
                    event.mouseMove.x = mouseEvent.x;
                    event.mouseMove.y = mouseEvent.y;
                    return true;
                }
            }
        }

        if (ch == ERR) {
            return false;
        }

        event.type = EventType::KeyPressed;

        switch (ch) {
            case KEY_UP:
                event.key.keyCode = KeyCode::Up;
                break;
            case KEY_DOWN:
                event.key.keyCode = KeyCode::Down;
                break;
            case KEY_LEFT:
                event.key.keyCode = KeyCode::Left;
                break;
            case KEY_RIGHT:
                event.key.keyCode = KeyCode::Right;
                break;
            case '\t':
                event.key.keyCode = KeyCode::Tab;
                break;
            case '\n':
                event.key.keyCode = KeyCode::Enter;
                break;
            case ' ':
                event.key.keyCode = KeyCode::Space;
                break;
            case 27:
                event.key.keyCode = KeyCode::Escape;
                break;
            case KEY_BACKSPACE:
            case 127:
                event.key.keyCode = KeyCode::Backspace;
                break;
            case KEY_SLEFT:
                event.key.keyCode = KeyCode::Shift;
                break;
            default:
                if (ch >= 'a' && ch <= 'z') {
                    event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (ch - 'a'));
                } else if (ch >= 'A' && ch <= 'Z') {
                    event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (ch - 'A'));
                } else if (ch >= '0' && ch <= '9') {
                    event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + (ch - '0'));
                } else {
                    event.key.keyCode = KeyCode::Unknown;
                }
                break;
        }
        return true;
    }

    void NCursesModule::drawRect(int x, int y, int width, int height, const Color &color)
    {
        attron(getColorPair(color));

        for (int i = 0; i < height; ++i) {
            if (y + i >= 0 && y + i < this->height) {
                for (int j = 0; j < width; ++j) {
                    if (x + j >= 0 && x + j < this->width) {
                        mvaddch(y + i, x + j, ' ' | A_REVERSE);
                    }
                }
            }
        }
        attroff(getColorPair(color));
    }

    void NCursesModule::drawCircle(int x, int y, int radius, const Color &color)
    {
        attron(getColorPair(color));

        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                if (i * i + j * j <= radius * radius) {
                    int drawY = y + i;
                    int drawX = x + j;

                    if (drawY >= 0 && drawY < this->height && drawX >= 0 && drawX < this->width) {
                        mvaddch(drawY, drawX, ' ' | A_REVERSE);
                    }
                }
            }
        }
        attroff(getColorPair(color));
    }

    void NCursesModule::drawText(int x, int y, const std::string &text, const Color &color)
    {
    getmaxyx(stdscr, height, width);
        if (y < 0 || y >= this->height) {
            return;
        }

        attron(getColorPair(color));

        mvprintw(y, x, "%s", text.c_str());

        attroff(getColorPair(color));
    }

    extern "C" {
        IDisplayModule *createDisplayModule() {
            return new NCursesModule();
        }
    }
}