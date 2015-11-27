#include "Client.h"
#include "MyClient.h"
#include "parser.h"
#include "State.h"

#include <map>

void printMatchResult(PARSER::eMatchResult r) {
    switch (r) {
        default: break;
        case PARSER::VICTORY: std::cout << "Victory" << std::endl; break;
        case PARSER::DEFEAT: std::cout << "Defeat" << std::endl; break;
        case PARSER::DRAW: std::cout << "Draw" << std::endl; break;
    }
}

std::string MyClient::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
    PARSER parser;
    parser.Parse(ServerResponse);

    state = State::fromParser(parser);
    answer.str("");

    /// --------------

    Weights plan;
    plan.add_source(19, 19, 40, [](int o, int d) { return o + 2*(80 - d); });
    plan.add_source(0, 19, 20, [](int o, int d) { return o + (20 - d); });
    plan.add_source(19, 0, 20, [](int o, int d) { return o + (20 - d); });

    std::map<Type, Weights> instincts;
    for (Type type : {ROCK, PAPER, SCISSORS}) {
        Weights instinct;
        forEnemies([&](const Unit& u) {
            if (u.type == getWinner(type)) {
                instinct.add_source(u.x, u.y, 4, [](int o, int d) { return o + (4 - d); });
            } else if (u.type == getBeater(type)) {
                instinct.add_source(u.x, u.y, 4, [](int o, int d) { return o - (4 - d); });
            } else {
                //same
            }
        });
        instincts[type] = instinct;
    }

    forOurs([&](const Unit& u) {
        const Weights& instinct = instincts[u.type];
        if (instinct.hasNonZero(u.x, u.y)) {
            answer << move(u, instinct.getWarmest(u.x, u.y));
        } else {
            answer << move(u, plan.getWarmest(u.x, u.y));
        }
    });

    answer << produce(typeFromInt(state.getOurTick() % 3));
    answer << ".";
    mDebugLog << answer.str() << std::endl;

    /// --------------

    // mDebugLog << "Plan:" << std::endl;
    mDebugLog << plan << std::endl;
    mDebugLog << state << std::endl;

    printMatchResult(parser.match_result);
    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
