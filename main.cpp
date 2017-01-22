#include <iostream>
#include <stdlib.h>
#include "TaskFeeder.h"
#include "MonteCarlo.h"
#include "Manager.h"
#define SEED 35791246

using namespace std;



int main()
{

    TaskFeeder feeder;
    Manager* manager;
    int totalNumOfTasks = 9000,i, hits=0, taskForEachFeeder;
    Task  **tasks1,**tasks2,**tasks3;

    //init rand
     srand((unsigned)time(NULL));


    Manager::init(5);
    manager = Manager::getInstance();


    manager->startThread();

    taskForEachFeeder = totalNumOfTasks /3 ;

    tasks1 = new Task*[taskForEachFeeder ];
    for(i=0 ; i<taskForEachFeeder ; i++){
        tasks1[i] = new MonteCarlo();
    }
    feeder.feedTasks(tasks1, taskForEachFeeder );


    tasks2 = new Task*[taskForEachFeeder ];
    for(i=0 ; i<taskForEachFeeder ; i++){
        tasks2[i] = new MonteCarlo();
    }
    feeder.feedTasks(tasks2, taskForEachFeeder );


    tasks3 = new Task*[taskForEachFeeder ];
    for(i=0 ; i<taskForEachFeeder ; i++){
        tasks3[i] = new MonteCarlo();
    }
    feeder.feedTasks(tasks3, taskForEachFeeder );


    //Need to wait for all Tasks to be compited before Calculate PI
    while(threadsTaskscompleted < totalNumOfTasks){}


    for(i=0 ; i<taskForEachFeeder  ; i++){
        hits += ((MonteCarlo*)tasks1[i])->getResult();
        hits += ((MonteCarlo*)tasks2[i])->getResult();
        hits += ((MonteCarlo*)tasks3[i])->getResult();
    }
    cout << "PI = " << MonteCarlo::calcPI(hits, totalNumOfTasks) << endl;


    Manager::logf.close();
    return 0;
}

