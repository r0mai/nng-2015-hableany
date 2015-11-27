#include "MyClient.h"

int main(int argc, char* argv[])
{
    std::cout.sync_with_stdio(false);
    std::string server_address;
    if (argc<2)
    {
        server_address = "172.22.22.48";
        std::cout<<"using default server address: " + server_address <<std::endl;
    } else
    {
        server_address = argv[1];
    }
    CLIENT *pClient = CreateClient();
    /* for debugging:  */
    std::ifstream debug_file("test.txt");
    if (debug_file.is_open())
    {
        std::string line;
        std::vector<std::string> full;
        while (std::getline(debug_file, line))
        {
            full.push_back(line);
        }
        std::string resp = pClient->DebugResponse(full);
        std::cout<<"response: "<<resp <<std::endl;
    }
    /**/

    if (!pClient->Init(server_address))
    {
        std::cout<<"Connection failed"<<std::endl;
    } else
    {
        pClient->Run();
    }
    delete pClient;
    return 0;
}
