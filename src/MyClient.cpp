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
    mDebugLog << state << std::endl;

    std::stringstream ss;
    ss<<"prod R\n";
    ss<<".";
    return ss.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
