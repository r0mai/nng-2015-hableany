#pragma once
#include "Client.h"



class MyClient : public CLIENT
{
public:
    MyClient();
protected:
    virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse);
    virtual std::string GetPassword() { return std::string("3hU2q8"); } // a kis hableany baratai
    virtual std::string GetPreferredOpponents() { return std::string("bot"); }
    virtual bool NeedDebugLog() { return true; }

};