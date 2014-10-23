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
#include <sstream>
#include <iomanip>

class Date {
private:
    int day;
    int month;
    int year;
    std::string deadline;
public:
    Date(){}
    ~Date(){}
    void setDeadline(std::string);
    void getCurrentDate();
    void getDeadline();
};


#endif /* defined(____Date__) */
