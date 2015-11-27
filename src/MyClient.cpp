#include "Client.h"
#include "MyClient.h"
#include "parser.h"
#include "State.h"

MyClient::MyClient()
{
}

std::string MyClient::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
    PARSER parser;
    parser.Parse(ServerResponse);

    State state = State::fromParser(parser);

    std::stringstream ss;
    ss << produce(typeFromInt(state.getOurTick() % 3)) << std::endl;
    ss<<".";
    mDebugLog << ss.str() << std::endl;

    mDebugLog << state << std::endl;

    return ss.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
