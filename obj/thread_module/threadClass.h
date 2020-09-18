//
// Created by Vlad on 9/18/2020.
//

#ifndef UNTITLED2_THREADCLASS_H
#define UNTITLED2_THREADCLASS_H


class ThreadBase{
public:
    static void * threadBaseStart ( void * );
    virtual void run () = 0;
};


#endif //UNTITLED2_THREADCLASS_H
