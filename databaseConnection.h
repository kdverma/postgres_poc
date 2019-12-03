#include<iostream>
#include <string>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;

class databaseConnection
{
    public:
    databaseConnection()
    {

    }
    void getDbConnection(string hostName,string port,string dbName,string user)
    {
        string dbConnectionStr = "host=";
        dbConnectionStr.append(hostName);
        dbConnectionStr.append(" port=");
        dbConnectionStr.append(port);
        dbConnectionStr.append(" dbname=");
        dbConnectionStr.append(dbName);
        dbConnectionStr.append(" user=");
        dbConnectionStr.append(user);
        cout << dbConnectionStr << endl;
        pqxx::connection C("dbname=kapildb user=kapil");
        
        /*if(CONNECTION_MADE == PQstatus(C));
        {
            std::cout << "Connected to " << C.dbname() << std::endl;
        }
        else
        {
            std::cout << "Connection to " << C.dbname() << " failed" std::endl;
        }*/
        //return C;
    }

};