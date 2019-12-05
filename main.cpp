#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <thread>
#include <pqxx/pqxx>
#include "databaseConnection.h"
#include "databaseAPI.h"

int main()
{
    try
    {
        long id;
        std::vector<std::thread> writterThreads;
        std::vector<std::thread> readerThreads;
        for(id = 1; id <= 5; id++) 
        {     
            writterThreads.emplace_back(std::thread(workerThreadInsert,id));
        }

        for(id = 1; id <= 5; id++) 
        {     
            readerThreads.emplace_back(std::thread(workerThreadRead,id));
        }

        
        printf("Waiting for thread to finish...\n");
        
        for (auto& t : writterThreads)
        {
            if(t.joinable()) 
            {
                t.join(); 
            }
        }

        for (auto& t : readerThreads)
        {
            if(t.joinable()) 
            {
                t.join(); 
            }
        }
    }
    catch (const std::exception &e)
    {
        cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
