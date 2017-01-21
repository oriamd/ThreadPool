#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>
#include "Task.h"

char* itoa(int val, int base);

using namespace std;

void* workerThread(void* arg);
void* managerThread(void* arg);

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t managerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t workingThreadCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t managerThreadCond = PTHREAD_COND_INITIALIZER;


//Queue who contains all tasks to be execute
queue<Task*> taskQueue;
//Total Tasks that has been completed
volatile int threadsTaskscompleted = 0;


class Manager{
    //list of all threadIds
    queue<pthread_t> threadList;

    //singleton instance
    static Manager* instance;
    //C'tor
    Manager(int numThreads){
        cout<<"C'tor Manager"<<endl;
        pthread_t newThreadId;
        Manager::numOfThreads = numThreads;
        int i;
        //Opening the log file
        logf.open("threadlog");
        //Allocting threadId's and creating Thread
        for(i =0 ; i < Manager::numOfThreads ; i++){
            if(pthread_create(&newThreadId, NULL, workerThread, NULL) == 0){
                threadList.push(newThreadId);
            }else{
                cout <<"Manager : Unable to create Thread"<<endl;
            }

        }

    }


public:
    //Num of threads running
    static int numOfThreads ;
    //Tell if ther are new tasks to spread
    static bool newTasks;
    //Log fd
    static ofstream logf;

    //Need to initialize before getting instance
    static void init(int numThreads){
        Manager::instance = new Manager(numThreads);
    }
    //singleton getInstance, Need to init first
    static Manager* getInstance(){
        return Manager::instance;
    }

    int startThread(){
        pthread_t newThread;
        return pthread_create(&newThread, NULL, managerThread, NULL);
    }

};

//init
int Manager::numOfThreads = 1;
Manager* Manager::instance = NULL;
bool Manager::newTasks = false;
ofstream Manager::logf ;

//WorkerThread main function
void* workerThread(void* arg){
    pthread_t threadId = pthread_self();
    cout << (unsigned)threadId <<" : Started"<<endl;
    Task* task;
    int result,numOfTask;

    while(true){

        //First locking
        pthread_mutex_lock(&queueMutex);
        // I reached the line
        while(taskQueue.empty()){
            //If the queue is empty so the thread will sleep and wait for a signal, signal might be that queue is not empty
            pthread_cond_wait(&workingThreadCond, &queueMutex);
        }
        // mutex is locked, now getting task from queue
        task = (Task*) taskQueue.front();
        taskQueue.pop();
        //inc the tasks counter
        numOfTask= ++threadsTaskscompleted;
        // Unlocking the Lock agine
        pthread_mutex_unlock(&queueMutex);

        // Excuting task
        result = task->execute();

        //Locking before writing to log
        pthread_mutex_lock(&fileMutex);
        //Writing to log when locked
        Manager::logf <<numOfTask<<": " <<(unsigned)threadId <<" Execute Task with result " << result << endl;
        //Now unlock when file is safe to write by other threads
        pthread_mutex_unlock(&fileMutex);
    }
    return NULL;
};


void* managerThread(void* arg){

    Manager* manager = Manager::getInstance();

    while(true){

        pthread_mutex_lock(&managerMutex);
        //Will chack if there are new Tasks to spread
        while(!Manager::newTasks){
            // waiting for condition
            pthread_cond_wait(&managerThreadCond, &managerMutex);
        }
        //Yes, new Tasks in
        Manager::newTasks = false;
        //signaling threads of new tasks in queue
        pthread_cond_signal(&workingThreadCond);
        //Now will unlock and wait for cond to change
        pthread_mutex_unlock(&managerMutex);
        cout <<"<Manager Thread : Tasks In, Notifying WorkingThreads" << endl;

    }

    return NULL;

}

//Gets a int and returns it's based value in string
char* itoa(int val, int base){

	static char buf[32] = {0};

	int i = 30;

	for(; val && i ; --i, val /= base)

		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}


#endif
