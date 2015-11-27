#define WIDTH 20
#define HEIGHT 20

class CLIENT
{
public:
	struct PLAYER
	{
		int id;
		int match_wins;
		int elo_points;
		std::string name;
	};
	std::vector<PLAYER> Players;
	void ParsePlayers(std::vector<std::string> &ServerResponse);

	CLIENT();
	virtual ~CLIENT();
	// Initializes the connection to the given server and port
	bool Init( std::string aIPAddress );

	// Runs the client
	void Run();

	std::string DebugResponse(std::vector<std::string> &text) { return HandleServerResponse(text); }

protected:
	void SendMessage( std::string aMessage );
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse) = 0;
	virtual void ConnectionClosed();
	virtual std::string GetPassword() = 0;
	virtual std::string GetPreferredOpponents() = 0;
	virtual bool NeedDebugLog() = 0;
	std::ofstream mDebugLog;
#ifdef WIN32
	SOCKET mConnectionSocket;
#else
	int mConnectionSocket;
#endif
};

CLIENT *CreateClient();
