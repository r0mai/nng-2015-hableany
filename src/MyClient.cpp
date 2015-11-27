#include "Client.h"
#include "MyClient.h"
#include "parser.h"


MYCLIENT::MYCLIENT()
{
}

std::string MYCLIENT::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
	PARSER parser;
	parser.Parse(ServerResponse);
	std::stringstream ss;
	ss<<"prod R\n";
	ss<<".";
	return ss.str();
}


CLIENT *CreateClient()
{
	return new MYCLIENT();
}
