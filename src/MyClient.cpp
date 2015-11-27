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

std::function<int(int, int)> heat(int amount) {
    return [&](int o, int d) {
        return o + (amount - d);
    };
}

std::function<int(int, int)> cool(int amount) {
    return [&](int o, int d) {
        return o - (amount - d);
    };
}


std::string MyClient::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
    PARSER parser;
    parser.Parse(ServerResponse);

    state = State::fromParser(parser);
    answer.str("");

    /// --------------

    std::map<Type, Weights> plans;
    for (Type type : {ROCK, PAPER, SCISSORS}) {
        Weights plan;
        plan.add_source(19, 19, 40, heat(700));

        if (state.rightTopBase == NEUTRAL) {
            plan.add_source(19, 0, 20, heat(400));
        } else if (state.rightTopBase == THEIRS) {
            plan.add_source(19, 0, 20, heat(400));
        } else {
            plan.add_source(19, 0, 20, cool(400));
        }
        forEnemies([&](const Unit& u) {
            if (u.type == getWinner(type)) {

            } else if (u.type == getBeater(type)) {
                plan.add_source(u.x, u.y, 15, cool(30));
            } else {

            }
        });

        // OVERWRITE EVERYTHING. TODO remove

        plan = Weights();
        plan.add_source(19, 19, 40, heat(700));
        plan.add_source(19, 0, 20, cool(400));
        plan.add_source(0, 19, 20, cool(400));

        plans[type] = plan;
    }

    std::map<Type, Weights> instincts;
    for (Type type : {ROCK, PAPER, SCISSORS}) {
        Weights instinct;
        forEnemies([&](const Unit& u) {
            if (u.type == getWinner(type)) {
                instinct.add_source(u.x, u.y, 4, [](int o, int d) { return o + (4 - d); });
            } else if (u.type == getBeater(type)) {
                instinct.add_source(u.x, u.y, 4, [](int o, int d) { return o - (4 - d); });
            } else {

            }
        });
        instincts[type] = instinct;
    }

    forOurs([&](const Unit& u) {
        const Weights& plan = plans[u.type];
        const Weights& instinct = instincts[u.type];
        if (instinct.hasNonZero(u.x, u.y)) {
            answer << move(u, plan.getWarmest(u.x, u.y));
        } else {
            answer << move(u, plan.getWarmest(u.x, u.y));
        }
    });

    answer << produce(typeFromInt(state.getOurTick() % 3));
    answer << ".";
    mDebugLog << answer.str() << std::endl;

    /// --------------

    // mDebugLog << "Plan:" << std::endl;
    mDebugLog << plans[ROCK] << std::endl;
    mDebugLog << state << std::endl;

	kif << "{";
	kif << "\"grid\": [";
	for(int i=0;i<state.units.size();++i)
	{
		kif << "[";
		for(int j=0;j<state.units[i].size();++j)
		{
			kif << "{ ";
			kif << "\"unit\": " << state.units[j][i] << ",";
			kif << "\"weight\": " << w.values[j][i];
			kif << "}";
			if(j<state.units[i].size()-1) kif << ",";
		}
		kif << "]";
		if(i<state.units.size()-1) kif << ",";
	}
	kif << "],";
	kif << "\"tick\":" << state.tick;
	kif << "},";


    printMatchResult(parser.match_result);
    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
