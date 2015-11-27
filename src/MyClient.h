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
    //virtual std::string GetPreferredOpponents() { return std::string("bot"); }
    virtual std::string GetPreferredOpponents() { return std::string("any"); }
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


    Unit *findUnit(int id) {
        for (unsigned y = 0; y < 20; ++y) {
            for (unsigned x = 0; x < 20; ++x) {
                if (state.units[x][y].type != EMPTY &&
                    state.units[x][y].id == id)
                {
                    return &state.units[x][y];
                }
            }
        }
        return nullptr;
    }

    void fillTemporaryShit() {
        if (firstOurId == -1) {
            forOurs([&](const Unit& u) {
                firstOurId = u.id;
            });
        }
        if (firstEnemyId == -1) {
            forEnemies([&](const Unit& u) {
                firstEnemyId = u.id;
            });
        }
        if (firstEnemyId == -1) {
            return;
        }
        if (firstOurId == -1) {
            return;
        }
        firstOurUnit = findUnit(firstOurId);
        firstEnemyUnit = findUnit(firstEnemyId);
    }

    int firstOurId = -1;
    int firstEnemyId = -1;

    Unit *firstOurUnit = nullptr;
    Unit *firstEnemyUnit = nullptr;

    std::stringstream answer;
};
