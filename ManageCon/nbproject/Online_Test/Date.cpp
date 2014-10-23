//
//  Date.cpp
//  
//
//  Created by Peter Mavridis on 23/10/2014.
//
//

#include "Date.h"

void Date::getCurrentDate() {
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::cout << "Current time and date: " << asctime(timeinfo) << std::endl;

}

void Date::setDeadline(std::string date){
    std::string tmp;
    std::stringstream stream(date);
    int i = 0;
    while (getline(stream, tmp, '/')) {
        switch (i) {
            case 0:
                day = atoi(tmp.c_str());
                i++;
                break;
            case 1:
                month = atoi(tmp.c_str());
                i++;
                break;
            case 2:
                year = atoi(tmp.c_str());
                i++;
                break;
            default:
                break;
        }
    }
}

void Date::getDeadline() {
    std::cout << std::setw(2) << std::setfill('0') << day << "/" << std::setw(2) << std::setfill('0') << month << "/" << year << std::endl;
    
}