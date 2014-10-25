//
//  Date.h
//  
//
//  Created by Peter Mavridis on 23/10/2014.
//
//

#ifndef Date_H
#define Date_H

#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

class Date 
{
	private:
		int day;
		int month;
		int year;
	public:
		Date(){}
		Date(int,int,int);
		Date(std::string);
		~Date(){}
		void setDeadline(std::string);
		void getCurrentDate();
		bool operator>=(const Date&);
		Date& operator+=(const int&);
		std::string toString();
		void toDate(std::string);
		friend std::ostream& operator<<(std::ostream&, const Date&);
};


#endif /* defined(Date_H) */
