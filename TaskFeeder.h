#ifndef TASKFEEDER_H
#define TASKFEEDER_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <queue>
#include "Manager.h"
#include "Task.h"


using namespace std;



class TaskFeeder{
public:
    void feedTasks(Task** tasks, int n){
        Manager* manager = Manager::getInstance();
        // Before Pushing tasks need to lock mutex
        pthread_mutex_lock(&queueMutex);
        for(int i=0; i<n ; i++){
            taskQueue.push(tasks[i]);
        }
        //Unlocking the Queue after done pushing new tasks
        pthread_mutex_unlock(&queueMutex);

        //Thread safe setting newTasks to true
        pthread_mutex_lock(&managerMutex);
        Manager::newTasks = true;
        pthread_mutex_unlock(&managerMutex);
        //Notifying the Manager of condition change
        pthread_cond_signal(&managerThreadCond);
    }

};

#endif // TASKFEEDER_H
