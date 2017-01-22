#ifndef MONTECARLO_H
#define MONTECARLO_H
#include "Task.h"


class MonteCarlo : public Task{
    //executee saves result
    bool result =0 ;

public:

    // "The Task" set result member to Hit or not */
    virtual int execute(){
        double x, y;
        unsigned int rand_state = rand();
        x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        if( (pow(x,2) + pow(y,2)) < 1 ){
            result = true;              // Hit
        }else{
            result = false;
        }
        return result;
    }

    //return the result of task ( operator() )
    bool getResult(){
        return result;
    }

    //calculate the PI with total numbers of Hits and totalIterators tries
    static double calcPI(int totalHit , int totalIterators ){
        return 4.0*(double)totalHit/totalIterators ;
    }

};


#endif // MONTECARLO_H_INCLUDED
