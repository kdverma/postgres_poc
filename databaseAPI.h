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
#include "binaryblob.h"
using namespace std;
using namespace pqxx;

#define BILLION 1000000000L
#define RECORDS_PER_THREAD (3251041/12)
//#define RECORDS_PER_THREAD 100

void add_session(pqxx::transaction_base &txn, std::string &cacheId, int &resourceSize, int &resourceCreated, string &resourceHeader, string &resourceUrl)
{
    void *bin_data = static_cast<void *>(rawDataOne);
    size_t bin_size = 4425; // -- ...and the size of the binary data
    pqxx::binarystring blob(bin_data, bin_size);

    txn.exec("INSERT INTO cachedbtable(cache_id, resource_size, resource_created, resource_headers, resource_url, resource_data) "
             "VALUES ( " +
             txn.quote(cacheId) + ", " +
             txn.quote(resourceSize) + ", " +
             txn.quote(resourceCreated) + ", " +
             txn.quote(resourceHeader) + ", " +
             txn.quote(resourceUrl) + ", " +
             txn.quote(blob) +
             ")");
}

void generateRandomData(string &cacheId, int &resourceSize, int &resourceCreated, string &resourceHeader, string &resourceUrl, int &arg)
{
    long int ns;
    time_t sec;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    sec = spec.tv_sec;
    ns = spec.tv_nsec;

    uint64_t randomNum = (uint64_t)sec * BILLION + (uint64_t)ns;

    cacheId = "";
    std::ostringstream oss;
    oss << randomNum;
    oss << "_" << arg;
    cacheId.append(oss.str());

    resourceSize = 50;
    resourceCreated = 50;
    resourceHeader = "resourceHeader";
    resourceUrl = "resourceURL";
}

void workerThreadInsert(int arg)
{
    //cout << "worker thread " << arg << endl;
    pqxx::connection C("dbname=cachedatabase user=postgres");
    //std::cout << "writter thread " << arg << " Connected to " << C.dbname() << std::endl;
    time_t startTime = time(0);
    printf("writter thread [%d] started at [%d] for [%d] entries\n", arg, startTime, RECORDS_PER_THREAD);
    try
    {
        int count = 0;
        while (count < RECORDS_PER_THREAD)
        {
            std::string cacheId;
            int resourceSize;
            int resourceCreated;
            string resourceHeader;
            string resourceUrl;

            generateRandomData(cacheId, resourceSize, resourceCreated, resourceHeader, resourceUrl, arg);
            pqxx::work txn{C};
            add_session(txn, cacheId, resourceSize, resourceCreated, resourceHeader, resourceUrl);
            txn.commit();

            /*sleep for 1/10 (100000 us) of second*/
            //usleep(100000);
            count++;
        }
        time_t endTime = time(0);
        printf("writter thread [%d] stopped at [%d] after inserting [%d] rows, time taken = [%d] seconds\n", arg, endTime, RECORDS_PER_THREAD, (endTime - startTime));
    }
    catch (const pqxx::sql_error &e)
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
    catch (...)
    {
        std::cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
}

void workerThreadRead(int arg)
{
    pqxx::connection C("dbname=cachedatabase user=postgres");
    //std::cout << "reader thread " << arg << " Connected to " << C.dbname() << std::endl;
    time_t startTime = time(0);
    printf("reader thread [%d] started at [%d] for [%d] entries\n", arg, startTime, RECORDS_PER_THREAD);
    try
    {
        int count = 0;
        while (count < RECORDS_PER_THREAD)
        {
            std::string sessionId;
            int pageSeq = 0;
            std::string cacheId;
            std::string eventId;

            pqxx::work txn{C};
            pqxx::result data = txn.exec("SELECT * FROM cachedbtable ORDER BY cache_id DESC limit 1");
            if (data.size() != 1)
            {
                printf("thread [%d] Could not read data from from db [%s] ",arg, C.dbname());
            }
            txn.commit();

            /*sleep for 1/10 (100000 us) of second*/
            //usleep(10000);
            count++;
        }
        time_t endTime = time(0);
        printf("reader thread [%d] stopped at [%d] after %d times read operation, time taken = [%d] seconds\n", arg, endTime, count, (endTime - startTime));
    }
    catch (const pqxx::sql_error &e)
    {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
    catch (...)
    {
        std::cout << "ERRRORRRRRRRRRRRRR" << endl;
        std::cerr << "thread number : " << arg << std::endl;
    }
}
