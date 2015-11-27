#include "Client.h"
#include "MyClient.h"
#include "parser.h"
#include "State.h"

std::string MyClient::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
    PARSER parser;
    parser.Parse(ServerResponse);

    state = State::fromParser(parser);
    answer.str("");

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

    mDebugLog << state << std::endl;

    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
