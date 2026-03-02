#ifndef GOMOKUENGINE_HPP
#define GOMOKUENGINE_HPP

#include <vector>
#include <utility>
#include <random>

class GomokuEngine {
public:
    GomokuEngine();
    ~GomokuEngine();

    void start(int size);
    void updateCell(int x, int y, int owner);
    
    std::pair<int, int> computeMove();

    int getSize() const;
    bool isValid(int x, int y) const;
    bool isCellEmpty(int x, int y) const;

private:
    int _size;
    std::vector<std::vector<int>> _board;
    
    std::mt19937 _rng;
};

#endif