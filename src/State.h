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

