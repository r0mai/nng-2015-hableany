#include "stdafx.h"
#include "Client.h"
#include "parser.h"
#include <cassert>

PARSER::PARSER()
{
	tick = 0;
	versus = 0;
	prod_limit = 0;
	match_result = PARSER::ONGOING;
	prod[0] = prod[1] = prod[2] = 0;
	base_owner[0] = base_owner[1] = -1;
}

void PARSER::Parse(const std::vector<std::string> &ServerResponse)
{
	tick = 0;
	soldiers.clear();
	match_result = PARSER::ONGOING;
	int i;
	for(i=0;i<(int)ServerResponse.size();i++)
	{
		if (ServerResponse[i].substr(0, 8)=="soldiers")
		{
			int count = atoi(ServerResponse[i].substr(9).c_str());
			soldiers.resize(count);
			int r;
			for(r=0;r<count;r++)
			{
				char c1, c2;
				sscanf(ServerResponse[i+r+1].c_str(), "%d %c %c %d %d", &soldiers[r].id, &c1, &c2, &soldiers[r].x, &soldiers[r].y);
				assert(c1=='A' || c1=='E');
				assert(c2=='R' || c2=='P' || c2=='S');
				assert(soldiers[r].x>=0 && soldiers[r].x<WIDTH);
				assert(soldiers[r].y>=0 && soldiers[r].y<HEIGHT);
				soldiers[r].side = c1=='E';
				soldiers[r].t = c2=='R'?0:c2=='P'?1:2;
			}
			i+=count;
		} else if (ServerResponse[i].substr(0, 4)=="tick")
		{
			tick = atoi(ServerResponse[i].substr(5).c_str());
		} else if (ServerResponse[i].substr(0, 6)=="versus")
		{
			std::string str_vs = ServerResponse[i].substr(7);
			if (str_vs == "bot") versus = -1;
			else versus = atoi(str_vs.c_str());
		} else if (ServerResponse[i].substr(0, 4)=="base")
		{
			char c1=ServerResponse[i][5];
			char c2=ServerResponse[i][7];
			assert(c1=='r' || c1=='d');
			assert(c2=='N' || c2=='A' || c2=='E');
			base_owner[c1=='r'?0:1]=c2=='N'?-1:c2=='A'?0:1;
		} else if (ServerResponse[i].substr(0, 4)=="prod")
		{
			sscanf(ServerResponse[i].substr(5).c_str(), "%d %d %d %d", &prod_limit, &prod[0], &prod[1], &prod[2]);
		} else if (ServerResponse[i].substr(0, 8)=="finished")
		{
			std::string res = ServerResponse[i].substr(9);
			if (res=="victory")
				match_result = PARSER::VICTORY;
			else if (res=="draw")
				match_result = PARSER::DRAW;
			else if (res=="defeat")
				match_result = PARSER::DEFEAT;
		}
	}
}

