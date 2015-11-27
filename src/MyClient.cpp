#include "Client.h"
#include "MyClient.h"
#include "parser.h"
#include "State.h"


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
    plan.add_source(19, 19, 40, [](int o, int d) {
        return o + (40 - d);
    });

    forOurs([&](const Unit& u) {
        answer << move(u, plan.getWarmest(u.x, u.y));
    });

    answer << produce(typeFromInt(state.getOurTick() % 3));
    answer << ".";
    mDebugLog << answer.str() << std::endl;

    /// --------------

    mDebugLog << "Plan:" << std::endl;
    mDebugLog << plan << std::endl;
    mDebugLog << state << std::endl;

    printMatchResult(parser.match_result);
    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
