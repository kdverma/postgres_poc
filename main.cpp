#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <pqxx/pqxx>

int main()
{
    try
    {
        pqxx::connection C("dbname=kapildb user=kapil");
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT * FROM person");

        std::cout << "Found " << R.size() << " persons:" << std::endl;
        for (auto row: R){
            std::cout << row[0].c_str() << std::endl;
            std::cout << row[1].c_str() << std::endl;
            std::cout << row[2].c_str() << std::endl;
            std::cout << row[3].c_str() << std::endl;
            std::cout << row[4].c_str() << std::endl;
        }

       /* std::cout << "Doubling all employees' salaries..." << std::endl;
        W.exec("UPDATE person SET last_name=DEV");

        std::cout << "Making changes definite: ";
        W.commit();
        std::cout << "OK." << std::endl;

        std::cout << "check data again." << std::endl;
        R = W.exec("SELECT * FROM person");

        std::cout << "Found " << R.size() << "employees:" << std::endl;
        for (auto row: R)
            std::cout << row[0].c_str() << std::endl;*/


    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
