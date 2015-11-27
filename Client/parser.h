struct SOLDIER {
	int id, t, x, y, side;
};

class PARSER
{
public:
	PARSER();
	int tick;
	int versus; // enemy ID, or -1: bot
	int prod_limit; // unmodified ticks to finish produce
	int prod[3]; // produce state of R, P, S
	std::vector<SOLDIER> soldiers;
	int base_owner[2]; // {right, down} base owner, -1: neutral, 0: own, 1: enemy
	enum eMatchResult {
		ONGOING,
		VICTORY,
		DRAW,
		DEFEAT
	};
	eMatchResult match_result;
	void Parse(const std::vector<std::string> &ServerResponse);
};
