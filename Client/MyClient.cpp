#include "stdafx.h"
#include "Client.h"
#include "parser.h"

// sample

class MYCLIENT : public CLIENT
{
public:
	MYCLIENT();
protected:
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse);
	virtual std::string GetPassword() { return std::string("3hU2q8"); } // a kis hableany baratai
	virtual std::string GetPreferredOpponents() { return std::string("bot"); }
	virtual bool NeedDebugLog() { return true; }
};

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
