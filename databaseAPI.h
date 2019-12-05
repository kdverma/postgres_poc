#include <sstream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;

#define BILLION  1000000000L

struct sessionObject{
    //pqxx::transaction_base txn;
    std::string sessionId;
    int pageSeq;
    std::string cacheId;
    std::string eventId;
    int time;
};

pqxx::result get_all_sessions_data(pqxx::transaction_base &txn)
{
  // Execute database query and immediately retrieve results.
  return txn.exec("SELECT id, name, department FROM Employee");
}

void add_session(pqxx::transaction_base &txn, std::string sessionId, int pageSeq, string cacheId, string eventId, int time)
{
  // Use quote() to escape and quote a value safely for use in a
  // query.  Avoid bugs and security holes when strings contain
  // backslashes, quotes, or other "weird" characters.
  // That's not very useful for department (an integer), but it
  // also represents the value as a string.  We don't want to add
  // an int to the query, we want to add a string that represents
  // the int value.

    txn.exec(
    "INSERT INTO sessiontable(session_id,page_sequence,cache_id,event_id,page_timestamp) "
    "VALUES (" +
    txn.quote(sessionId) + ", " +
    txn.quote(pageSeq) + ", " +
    txn.quote(cacheId) + ", " +
    txn.quote(eventId) + ", " +
    txn.quote(time) +
    ")");
}

void read_session(pqxx::transaction_base &txn, std::string sessionId, int pageSeq, string cacheId, string eventId, int time)
{
    pqxx::result data = txn.exec("SELECT * FROM sessiontable ORDER BY session_id DESC limit 1");
    std::cout << "Found " << data.size() << " entries :" << std::endl;
    std::cout << "session_id\t\t\t" << "page_sequence\t\t\t" << "cache_id\t\t\t" << "event_id\t\t" << "page_timestamp\n";
    std::cout << "===============================================================================================================================================\n\n";
    for (auto row: data)
    {
        std::cout << row[0].c_str() <<"\t\t" << row[1].c_str() <<"\t\t\t"  << row[2].c_str() <<"\t\t" << row[3].c_str() <<"\t\t" << row[4].c_str() << std::endl;
    }
    /*for (auto row: data)
    {
        std::cout
        << row["session_id"].as<std::string>() << "\t"
        << row["page_sequence"].as<int>() << "\t"
        << row["cache_id"].as<std::string>() << "\t"
        << row["event_id"].as<std::string>() << "\t"
        << row["page_timestamp"].as<int>() << std:endl
        <<"==============================================================================="
        << std::endl;
    }*/

}

void generateRandomData(std::string &sessionId, int &pageSeq, string &cacheId, string &eventId, int &timeStamp)
{
    long int ns;
    //uint64_t all;
    time_t sec;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    sec = spec.tv_sec;
    ns = spec.tv_nsec;

    uint64_t randomNum = (uint64_t) sec * BILLION + (uint64_t) ns;

    //sessionId = "session_";
    sessionId = "";
    std::ostringstream oss;
    oss << randomNum;
    sessionId.append(oss.str());

    pageSeq = randomNum%1000000;

    cacheId = "cacheId_";
    cacheId.append(oss.str());

    eventId = "eventId_";
    eventId.append(oss.str());

    timeStamp = time(0);
}

void workerThreadInsert(int arg)
{
    cout << "worker thread " << arg << endl;
    pqxx::connection C("dbname=sessiondatabase user=kapil");
    std::cout << "Connected to " << C.dbname() << std::endl;
    //pqxx:: work txn{C};
    
    int count = 0;
    while(count < 50)
    {
        std::string sessionId;
        int pageSeq;
        std::string cacheId;
        std::string eventId;
        int time;

        generateRandomData(sessionId,pageSeq,cacheId,eventId,time);
        pqxx:: work txn{C};
        add_session(txn,sessionId,pageSeq,cacheId,eventId,/*time*/arg);
        txn.commit();

        /*sleep for 1/10 (100000 us) of second*/
        usleep(100000);
        count++;
    }
}

void workerThreadRead(int arg)
{
    pqxx::connection C("dbname=sessiondatabase user=kapil");

    int count = 0;
    while(count < 50)
    {
        cout << "\n\n\nworker thread read by thread : " << arg << endl;
        std::string sessionId;
        int pageSeq = 0;
        std::string cacheId;
        std::string eventId;
        
        pqxx:: work txn{C};
        read_session(txn,sessionId,pageSeq,cacheId,eventId,/*time*/arg);
        txn.commit();

        /*sleep for 1/10 (100000 us) of second*/
        usleep(100000);
        count++;
    }
}

