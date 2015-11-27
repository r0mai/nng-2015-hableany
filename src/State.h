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

// from 0, 1, 2
Type typeFromInt(int i);

std::string produce(Type t);

struct Unit {
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

std::ostream& operator<<(std::ostream& os, const Unit& u);
std::ostream& operator<<(std::ostream& os, const State& state);

