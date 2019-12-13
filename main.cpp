//#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <thread>
#include <pqxx/pqxx>
#include "databaseConnection.h"
#include "databaseAPI.h"

#define NUM_OF_THREAD 12
int main()
{
    
    try
    {
        long id;
        std::vector<std::thread> writterThreads;
        std::vector<std::thread> readerThreads;
        for (id = 1; id <= NUM_OF_THREAD; id++)
        {
            writterThreads.emplace_back(std::thread(workerThreadInsert, id));
        }

        for (id = 1; id <= NUM_OF_THREAD; id++)
        {
            readerThreads.emplace_back(std::thread(workerThreadRead, id));
        }

        printf("Waiting for thread to finish...\n");

        for (auto &t : writterThreads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }

        for (auto &t : readerThreads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }
    catch (const pqxx::sql_error &e)
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
