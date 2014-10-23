//
//  Date.h
//  
//
//  Created by Peter Mavridis on 23/10/2014.
//
//

#ifndef ____Date__
#define ____Date__

#include <iostream>
#include <time.h>
#include <string>

class Date {
private:
    int day;
    int month;
    int year;
public:
    Date(){}
    ~Date(){}
    void setDeadline(std::string);
    void getDeadline();
    void getCurrentDate();
};


#endif /* defined(____Date__) */
