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

std::function<double(double, int)> heat(double amount, double slope = 1) {
    return [=](double o, int d) {
        return o + (amount - slope*d);
    };
}

std::function<double(double, int)> heat_max(double amount, double slope = 1) {
    return [=](double o, int d) {
        return std::max(o,  (amount - slope*d));
    };
}

std::function<double(double, int)> cool(double amount, double slope = 1) {
    return [=](double o, int d) {
        return o - (amount - slope*d);
    };
}


std::string MyClient::HandleServerResponse(std::vector<std::string> &ServerResponse)
{
    PARSER parser;
    parser.Parse(ServerResponse);

    state = State::fromParser(parser);
    fillTemporaryShit();
    answer.str("");

    /// --------------

    Unit closestEnemyToBase = state.closestEnemy(0, 0);
    Unit closestEnemyToEnemyBase = state.closestEnemy(19, 19);
    Unit closestEnemyToTopRight = state.closestEnemy(19, 0);
    Unit closestEnemyToBottomLeft = state.closestEnemy(0, 19);

    std::map<Type, Weights> plans;
    for (Type type : {ROCK, PAPER, SCISSORS}) {
        Weights plan;

        if (closestEnemyToBase.type == getWinner(type) &&
            distance(closestEnemyToBase.x, closestEnemyToBase.y, 0, 0) < 10)
        {
            plan.add_source(0, 0, 29, heat_max(2000, 100));
        } else {
            // not cooling
        }

        if (state.leftBottomBase == NEUTRAL && state.rightTopBase == NEUTRAL) {
            plan.add_source(0, 19, 21, heat_max(1000, 10));
            plan.add_source(19, 0, 21, heat_max(1002, 10));
        } else if (state.rightTopBase == THEIRS && state.leftBottomBase == THEIRS) {
            plan.add_source(19, 0, 21, heat_max(1000, 10));
            plan.add_source(0, 19, 21, heat_max(1002, 10));
        } else if (state.rightTopBase == THEIRS && state.leftBottomBase == NEUTRAL) {
            plan.add_source(19, 0, 21, heat_max(1000, 10));
            plan.add_source(0, 19, 21, heat_max(1000, 10));
        } else if (state.rightTopBase == NEUTRAL && state.leftBottomBase == THEIRS) {
            plan.add_source(19, 0, 21, heat_max(1000, 10));
            plan.add_source(0, 19, 21, heat_max(1000, 10));
        } else if (state.rightTopBase == OURS &&
            (state.leftBottomBase == THEIRS || state.leftBottomBase == NEUTRAL))
        {
            if (closestEnemyToTopRight.type == getWinner(type) &&
                distance(closestEnemyToTopRight.x, closestEnemyToTopRight.y, 0, 0) < 6)
            {
                plan.add_source(19, 0, 21, heat_max(1300, 10));
            } else {
                plan.add_source(0, 19, 21, heat_max(1500, 10));
                plan.add_source(19, 19, 21, heat_max(1400, 10));
            }
        } else if ((state.rightTopBase == THEIRS || state.rightTopBase == NEUTRAL) &&
                state.leftBottomBase == OURS)
        {
            if (closestEnemyToBottomLeft.type == getWinner(type) &&
                distance(closestEnemyToBottomLeft.x, closestEnemyToBottomLeft.y, 0, 0) < 6)
            {
                plan.add_source(0, 19, 21, heat_max(1300, 10));
            } else {
                plan.add_source(19, 0, 21, heat_max(1500, 10));
                plan.add_source(19, 19, 21, heat_max(1400, 10));
            }
        } else if (state.rightTopBase == OURS && state.leftBottomBase == OURS) {
            if (closestEnemyToBottomLeft.type == getWinner(type) &&
                distance(closestEnemyToBottomLeft.x, closestEnemyToBottomLeft.y, 0, 0) < 6)
            {
                plan.add_source(0, 19, 21, heat_max(1300, 10));
            } else if (closestEnemyToTopRight.type == getWinner(type) &&
                distance(closestEnemyToTopRight.x, closestEnemyToTopRight.y, 0, 0) < 6)
            {
                plan.add_source(19, 0, 21, heat_max(1300, 10));
            }

            plan.add_source(19, 19, 41, heat_max(1500, 15));
        } else {
            mDebugLog << "IMPOSSIBLE CASE" << std::endl;
        }

        forOurs([&](const Unit& u) {
            plan.set(u.x, u.y, -1.0);
        });

        plans[type] = plan;
    }

    std::map<Type, Weights> instincts;
    for (Type type : {ROCK, PAPER, SCISSORS}) {
        Weights instinct;
        forEnemies([&](const Unit& u) {
            if (u.type == getWinner(type)) {
                instinct.add_source(u.x, u.y, 4, heat(4));
            } else if (u.type == getBeater(type)) {
                instinct.add_source(u.x, u.y, 4, cool(4));
            } else {

            }
        });
        forOurs([&](const Unit& u) {
            instinct.set(u.x, u.y, -100.0);
        });
        instincts[type] = instinct;
    }

    forOurs([&](const Unit& u) {
        const Weights& plan = plans[u.type];
        const Weights& instinct = instincts[u.type];
        if (instinct.shouldConsider(u.x, u.y, -99.0)) {
            auto ans = move(u, instinct.getWarmest(u.x, u.y));
            answer << ans;
            mDebugLog << "instinct " << ans;
        } else {
            auto ans = move(u, plan.getWarmest(u.x, u.y));
            answer << ans;
            mDebugLog << "plan " << ans;
        }
    });
	
	auto allyMaxUnitType = state.getAllyMaxUnitType();

    if (closestEnemyToBase.type != EMPTY)
    {
		if(allyMaxUnitType != EMPTY)
		{
			if(allyMaxUnitType == getBeater(closestEnemyToBase.type))
			{
				auto ans = produce(closestEnemyToBase.type);
				answer << ans << ".";
			}
			else
			{
				auto ans = produce(getBeater(closestEnemyToBase.type));
				answer << ans << ".";
			}
		}
		else
		{
			auto ans = produce(getBeater(closestEnemyToBase.type));
			answer << ans << ".";
		}
    }
    else
    {
        auto ans = produce(typeFromInt(rand()%3));
        answer << ans << ".";
    }
    mDebugLog << answer.str() << std::endl;

    /// --------------

    // mDebugLog << "Plan:" << std::endl;
    mDebugLog << "ar:" << state.ar << "as:" << state.as << "ap:" << state.ap << std::endl;
    mDebugLog << "er:" << state.er << "es:" << state.es << "ep:" << state.ep << std::endl;
    mDebugLog << plans[ROCK] << std::endl;
    mDebugLog << instincts[ROCK] << std::endl;
    mDebugLog << state << std::endl;

    kif << "{";
    kif << "\"g\": [";
    for(int i=0;i<state.units.size();++i)
    {
        kif << "[";
        for(int j=0;j<state.units[i].size();++j)
        {
            kif << "{ ";
            kif << "\"u\": \"" << state.units[j][i] << "\",";
            kif << "\"w\": ";
            kif << "{";
            kif << "\"r\": " << plans[ROCK].values[j][i] << ",";
            kif << "\"p\": " << plans[PAPER].values[j][i] << ",";
            kif << "\"s\": " << plans[SCISSORS].values[j][i];
            kif << "}";
            kif << "}";
            if(j<state.units[i].size()-1) kif << ",";
        }
        kif << "]";
        if(i<state.units.size()-1) kif << ",";
    }
    kif << "],";
    kif << "\"t\":" << state.tick;
    kif << "},";


    printMatchResult(parser.match_result);
    return answer.str();
}


CLIENT *CreateClient()
{
    return new MyClient();
}
