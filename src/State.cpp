#include <vector>
#include "State.h"

State State::fromParser(const PARSER& parser) {
    State state;

    for (const auto& s : parser.soldiers) {
        Unit unit;
        unit.id = s.id;
        unit.type = s.t == 0 ? ROCK : (s.t == 1 ? PAPER : SCISSORS);
        unit.is_enemy = bool(s.side);
        state.units[s.x][s.y] = unit;
    }

    return state;
}

std::ostream& operator<<(std::ostream& os, const Unit& u) {
    char ch;
    switch (u.type) {
        default:
        case EMPTY: os << '.'; return os;
        case ROCK: ch = 'r'; break;
        case PAPER: ch = 'p'; break;
        case SCISSORS: ch = 's'; break;
    }
    if (!u.is_enemy) {
        ch = std::toupper(ch);
    }
    os << ch;
    return os;
}

std::ostream& operator<<(std::ostream& os, const State& state) {
    for (unsigned y = 0; y < 20; ++y) {
        for (unsigned x = 0; x < 20; ++x) {
            os << state.units[x][y];
        }
        os << '\n';
    }
    return os;
}
