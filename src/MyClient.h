#pragma once
#include "Client.h"
#include "State.h"


class MyClient : public CLIENT
{
public:
    MyClient() = default;
protected:
    virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse);
    virtual std::string GetPassword() { return std::string("3hU2q8"); } // a kis hableany baratai
    virtual std::string GetPreferredOpponents() { return std::string("bot"); }
    virtual bool NeedDebugLog() { return true; }

/////////////////////////////////////////////
    State state;

    template<class F>
    void forOurs(F f) {
        for (unsigned y = 0; y < 20; ++y) {
            for (unsigned x = 0; x < 20; ++x) {
                if (state.units[x][y].type != EMPTY &&
                    !state.units[x][y].is_enemy)
                {
                    f(state.units[x][y]);
                }
            }
        }
    }

    template<class F>
    void forEnemies(F f) {
        for (unsigned y = 0; y < 20; ++y) {
            for (unsigned x = 0; x < 20; ++x) {
                if (state.units[x][y].type != EMPTY &&
                    state.units[x][y].is_enemy)
                {
                    f(state.units[x][y]);
                }
            }
        }
    }

    std::stringstream answer;
};
