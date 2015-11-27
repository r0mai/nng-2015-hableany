#pragma once

#include <array>
#include <ostream>

#include "parser.h"

enum Type {
    EMPTY,
    ROCK,
    PAPER,
    SCISSORS
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

std::string toString(Direction d);

// from 0, 1, 2
Type typeFromInt(int i);

struct Unit {
    int x = -1;
    int y = -1;
    int id = -1;
    bool is_enemy = false;
    Type type = EMPTY;
};

struct State {
    int tick;

    int getOurTick() const;

    std::array<std::array<Unit, 20>, 20> units;

    static State fromParser(const PARSER& parser);
};

std::string produce(Type t);
std::string move(const Unit& u, Direction dir);


std::ostream& operator<<(std::ostream& os, const Unit& u);
std::ostream& operator<<(std::ostream& os, const State& state);

inline
int abs(int x) {
    if (x < 0) return -x;
    return x;
}

inline
int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

struct Weights {
    Weights() { fill(0); }

    void fill(int value);
    void set(int x, int y, int value);

    template<class F>
    void add_source(int sx, int sy, int r, F f) {
        for (int x = sx - r; x <= sx + r; ++x) {
            int y_count = r - abs(x - sx);
            for (int y = sy - y_count; y <= sy + y_count; ++y) {
                if (x >= 0 && x < 20 && y >= 0 && y < 20) {
                    values[x][y] = f(values[x][y], distance(sx, sy, x, y));
                }
            }
        }
    }

    std::array<std::array<int, 20>, 20> values;
};

std::ostream& operator<<(std::ostream& os, const Weights& w);
