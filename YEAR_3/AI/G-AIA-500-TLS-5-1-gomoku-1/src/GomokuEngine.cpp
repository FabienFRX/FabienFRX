#include "GomokuEngine.hpp"

GomokuEngine::GomokuEngine() : _size(20) {
    std::random_device rd;
    _rng = std::mt19937(rd());
    start(20);
}

GomokuEngine::~GomokuEngine() {}

void GomokuEngine::start(int size) {
    if (size <= 0) size = 20;
    _size = size;
    _board.assign(_size, std::vector<int>(_size, 0));
}

void GomokuEngine::updateCell(int x, int y, int owner) {
    if (isValid(x, y)) {
        _board[y][x] = owner;
    }
}

bool GomokuEngine::isValid(int x, int y) const {
    return (x >= 0 && x < _size && y >= 0 && y < _size);
}

bool GomokuEngine::isCellEmpty(int x, int y) const {
    return isValid(x, y) && _board[y][x] == 0;
}

std::pair<int, int> GomokuEngine::computeMove() {
    
    std::vector<std::pair<int, int>> emptyCells;
    for (int y = 0; y < _size; ++y) {
        for (int x = 0; x < _size; ++x) {
            if (_board[y][x] == 0) {
                emptyCells.emplace_back(x, y);
            }
        }
    }

    if (emptyCells.empty()) return {-1, -1};

    std::uniform_int_distribution<int> dist(0, emptyCells.size() - 1);
    return emptyCells[dist(_rng)];
}

int GomokuEngine::getSize() const { return _size; }
