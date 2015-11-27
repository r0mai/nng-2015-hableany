#include <vector>
#include <string>
#include "State.h"

std::string produce(Type t) {
    switch (t) {
        case ROCK: return "prod R";
        case PAPER: return "prod P";
        case SCISSORS: return "prod S";
        default: return "";
    }
}

std::string toString(Direction d) {
    switch (d) {
        case LEFT: return "left";
        case RIGHT: return "right";
        case UP: return "up";
        case DOWN: return "down";
    }
    return "";
}

std::string move(const Unit& u, Direction dir) {
    return std::to_string(u.id) + " " + toString(dir);
}

Type typeFromInt(int i) {
    return i == 0 ? ROCK : (i == 1 ? PAPER : SCISSORS);
}

State State::fromParser(const PARSER& parser) {
    State state;
    state.tick = parser.tick;

    for (const auto& s : parser.soldiers) {
        Unit unit;
        unit.x = s.x;
        unit.y = s.y;
        unit.id = s.id;
        unit.type = typeFromInt(s.t);
        unit.is_enemy = bool(s.side);
        state.units[s.x][s.y] = unit;
    }

    return state;
}

int State::getOurTick() const {
    return tick / 2;
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
    for (unsigned x = 0; x < 20 + 2; ++x) {
        os << '-';
    }
    os << '\n';
    for (unsigned y = 0; y < 20; ++y) {
        os << '|';
        for (unsigned x = 0; x < 20; ++x) {
            os << state.units[x][y];
        }
        os << '|';
        os << '\n';
    }
    for (unsigned x = 0; x < 20 + 2; ++x) {
        os << '-';
    }
    os << '\n';
    return os;
}
