//
//  Date.cpp
//  
//
//  Created by Peter Mavridis on 23/10/2014.
//
//

#include "Date.h"

int main(){
    Date date;
    
    date.getCurrentDate();
    return 0;
}

void Date::getCurrentDate() {
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::cout << "Current time and date: " << asctime(timeinfo) << std::endl;
    //printf ("Current local time and date: %s", asctime(timeinfo));

}

void Date::setDeadline(std::string date){
    for (int i = 0; i < strlen(date); i++) {
        <#statements#>
    }
    day = d;
    month = m;
    year = y;
}
int Date::getDeadline(int, int, int) {
    int deadline;
    
}