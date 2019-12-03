#include <sstream>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;

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

void generateRandomData(std::string &sessionId, int &pageSeq, string &cacheId, string &eventId, int &timeStamp)
{
    srand(time(0));
    int randomNum = rand();

    //randomNum = randomNum%100000;

    sessionId = "session_";
    std::ostringstream oss;
    oss << randomNum;
    sessionId.append(oss.str());

    pageSeq = randomNum;

    cacheId = "cacheId_";
    cacheId.append(oss.str());

    eventId = "eventId_";
    eventId.append(oss.str());

    timeStamp = time(0);


}

