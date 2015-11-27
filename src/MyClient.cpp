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

    Weights w;
    w.add_source(5, 4, 20, [](int o, int d) { return o + (20 - d); });
    w.add_source(15, 14, 20, [](int o, int d) { return o + (20 - d); });

    forOurs([this](const Unit& u) {
        if (u.x > u.y) {
            answer << move(u, DOWN);
        } else {
            answer << move(u, RIGHT);
        }
    });

    answer << produce(typeFromInt(state.getOurTick() % 3));
    answer << ".";
    mDebugLog << answer.str() << std::endl;

    /// --------------

    mDebugLog << w << std::endl;
    mDebugLog << state << std::endl;

    printMatchResult(parser.match_result);
    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
