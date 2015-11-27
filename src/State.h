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

struct Unit {
    int id = -1;
    bool is_enemy = false;
    Type type = EMPTY;
};

struct State {
    std::array<std::array<Unit, 20>, 20> units;

    static State fromParser(const PARSER& parser);
};

std::ostream& operator<<(std::ostream& os, const Unit& u);
std::ostream& operator<<(std::ostream& os, const State& state);

