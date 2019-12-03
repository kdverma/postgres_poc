#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <pqxx/pqxx>
#include "databaseConnection.h"
#include "databaseAPI.h"

int main()
{
    try
    {
        //databaseConnection dbConnection;
        //pqxx::Connection dbconn = dbConnection.getDbConnection("localhost","5432","kapildb","kapil");
        pqxx::connection C("dbname=sessiondatabase user=kapil");
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx:: work txn{C};
        //add_session(txn,"session_12",123,"cacheId_1","eventId_1",1241);

        pqxx::result R = txn.exec("SELECT * FROM sessiontable");

        std::cout << "Found " << R.size() << " entries :" << std::endl;
        std::cout << "session_id\t\t" << "page_sequence\t\t" << "cache_id\t\t" << "event_id\t\t" << "page_timestamp\n";
        std::cout << "============================================================================================================\n";
        for (auto row: R){
            std::cout << row[0].c_str() <<"\t\t" << row[1].c_str() <<"\t\t\t"  << row[2].c_str() <<"\t\t" << row[3].c_str() <<"\t\t" << row[4].c_str() << std::endl;
        }
        std::cout << "============================================================================================================\n";
        txn.commit();

        std::string sessionId;
        int pageSeq;
        std::string cacheId;
        std::string eventId;
        int time;

        generateRandomData(sessionId,pageSeq,cacheId,eventId,time);
        cout << sessionId << "\t" << pageSeq <<"\t\t" << cacheId << "\t" << eventId << "\t" << time << endl;
    }
    catch (const std::exception &e)
    {
        cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
