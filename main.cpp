#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <pqxx/pqxx>
#include "databaseConnection.h"
#include "databaseAPI.h"

int main()
{
    try
    {
        pthread_t my_thread[5];
        pthread_t my_thread_reader[5];
        void *ret_join;
        long id;
        for(id = 1; id <= 5; id++) 
        {
            int ret =  pthread_create(&my_thread[id], NULL, &workerThreadInsert, (void*)id);
            if(ret != 0) 
            {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }

        for(id = 1; id <= 5; id++) 
        {
            int ret =  pthread_create(&my_thread_reader[id], NULL, &workerThreadRead, (void*)id);
            if(ret != 0) 
            {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }

        printf("Waiting for thread to finish...\n");
        for(id = 1; id <= 5; id++) 
        {
            int ret = pthread_join(my_thread[id], &ret_join);
            if(ret != 0) 
            {
                perror("pthread_join failed writter thread");
                exit(EXIT_FAILURE);
            }
            printf("Thread joined for writter, it returned %s\n", (char *) ret_join);
            //exit(EXIT_SUCCESS);
        }

        for(id = 1; id <= 5; id++) 
        {
            int ret = pthread_join(my_thread_reader[id], &ret_join);
            if(ret != 0) 
            {
                perror("pthread_join failed for reader threads");
                exit(EXIT_FAILURE);
            }
            printf("Thread joined for reader, it returned %s\n", (char *) ret_join);
            //exit(EXIT_SUCCESS);
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
