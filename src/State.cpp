#include <vector>
#include <cassert>
#include <string>
#include "State.h"

// returns what defeats t
Type getBeater(Type t) {
    switch (t) {
        case ROCK: return PAPER;
        case PAPER: return SCISSORS;
        case SCISSORS: return ROCK;
        default: assert(false); return PAPER;
    }
}

// returns over what t wins
Type getWinner(Type t) {
    switch (t) {
        case PAPER: return ROCK;
        case SCISSORS: return PAPER;
        case ROCK: return SCISSORS;
        default: assert(false); return PAPER;
    }
}

std::string produce(Type t) {
    switch (t) {
        case ROCK: return "prod R\n";
        case PAPER: return "prod P\n";
        case SCISSORS: return "prod S\n";
        default: return "";
    }
}

std::string toString(Direction d) {
    switch (d) {
        case LEFT: return "left";
        case RIGHT: return "right";
        case UP: return "up";
        case DOWN: return "down";
        case STAY: return "";
    }
    return "";
}

std::string move(const Unit& u, Direction dir) {
    if (dir != STAY) {
        return std::to_string(u.id) + " " + toString(dir) + "\n";
    }
    return "";
}

Type typeFromInt(int i) {
    return i == 0 ? ROCK : (i == 1 ? PAPER : SCISSORS);
}

State State::fromParser(const PARSER& parser) {
    State state;
    state.ar=state.as=state.ap=state.er=state.es=state.ep=0;
    state.tick = parser.tick;
    state.rightTopBase = parser.base_owner[0] == -1 ? NEUTRAL :
        (parser.base_owner[0] == 0 ? OURS : THEIRS);
    state.leftBottomBase = parser.base_owner[1] == -1 ? NEUTRAL :
        (parser.base_owner[1] == 0 ? OURS : THEIRS);

    for (const auto& s : parser.soldiers) {
        Unit unit;
        unit.x = s.x;
        unit.y = s.y;
        unit.id = s.id;
        unit.type = typeFromInt(s.t);
        unit.is_enemy = bool(s.side);
        state.units[s.x][s.y] = unit;
        if(unit.is_enemy && unit.x == 19 && unit.y == 19 && unit.id > state.enemyLastId)
        {
            state.enemyLastId=unit.id;
            state.enemyProductionTime++;
        }
        if(unit.is_enemy)
        {
            switch(unit.type)
            {
                default: break;
                case ROCK: state.er++; break;
                case PAPER: state.ep++; break;
                case SCISSORS: state.es++; break;
            }
        }
        else
        {
            switch(unit.type)
            {
                default: break;
                case ROCK: state.ar++; break;
                case PAPER: state.ap++; break;
                case SCISSORS: state.as++; break;
            }
        }
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
	if(u.is_enemy)
	{
		os << "\e[0;34m";
	}
	else if(u.type!=EMPTY)
	{
		os << "\e[0;31m";
	}
    os << ch << "\e[0m";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Ownership& o) {
    switch (o) {
        case NEUTRAL: os << "\e[0m."; break;
        case OURS: os << "\e[0;31mB\e[0m"; break;
        case THEIRS: os << "\e[0;34mb\e[0m"; break;
    }
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
            if (state.units[x][y].type == EMPTY) {
                if (x == 19 && y == 0) {
                    os << state.rightTopBase;
                } else if (x == 0 && y == 19) {
                    os << state.leftBottomBase;
                } else {
                    os << state.units[x][y];
                }
            } else {
                os << state.units[x][y];
            }
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

void Weights::fill(double fill_value) {
    for (auto& column : values) {
        for (double& value : column) {
            value = fill_value;
        }
    }
}

void Weights::set(int x, int y, double value) {
    if (x < 0 || x >= 20 || y < 0 || y >= 20) {
        return;
    }
    values[x][y] = value;
}

std::ostream& operator<<(std::ostream& os, const Weights& w) {
    for (unsigned y = 0; y < 20; ++y) {
        for (unsigned x = 0; x < 20; ++x) {
            os << w.values[x][y] << ' ';
        }
        os << '\n';
    }
    return os;
}

Unit State::closestEnemy(int sx, int sy)
{
    int x=19,y=19;

    for(int i=0;i<units.size();++i)
    {
        for(int j=0;j<units[i].size();++j)
        {
            if(units[j][i].is_enemy && distance(sx,sy,x,y)>distance(sx,sy,j,i))
            {
                x=j;
                y=i;
            }
        }
    }
    return units[x][y];
}

int State::getAllyUnitSize(Type t)
{
    switch(t)
    {
        case EMPTY: return -1; break;
        case ROCK: return ar; break;
        case PAPER: return ap; break;
        case SCISSORS: return as; break;
    }
}

int State::getEnemyUnitSize(Type t)
{
    switch(t)
    {
        case EMPTY: return -1; break;
        case ROCK: return er; break;
        case PAPER: return ep; break;
        case SCISSORS: return es; break;
    }
}

Type State::getAllyMaxUnitType()
{
	if(ar>ap && ar>as) return ROCK;
	if(as>ap && as>ar) return SCISSORS;
	if(ap>as && ap>ar) return PAPER;
	return EMPTY;
	
}
Type State::getEnemyMaxUnitType()
{
	if(er>ep && er>es) return ROCK;
	if(es>ep && es>er) return SCISSORS;
	if(ep>es && ep>er) return PAPER;
	return EMPTY;
}
