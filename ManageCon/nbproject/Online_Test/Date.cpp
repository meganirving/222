//
//  Date.cpp
//  
//
//  Created by Peter Mavridis on 23/10/2014.
//
//

#include "Date.h"

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	os << std::right << std::setw(2) << std::setfill('0') << d.day << "/" << std::setw(2) << std::setfill('0') << d.month << "/" << std::setw(4) << std::setfill('0') << d.year;
	return os;
}

Date::Date(int newdays,int newmonths,int newyears)
{
	day = newdays;
	month = newmonths;
	year = newyears;
}

Date::Date(std::string os)
{
	int pos;
	std::string temp;
	
	pos = os.find_first_of("/");
	temp = (os.substr(0, pos));
	os = (os.substr(pos+1, os.length()-pos));
	day = atoi(temp.c_str());
	
	pos = os.find_first_of("/");
	temp = (os.substr(0, pos));
	os = (os.substr(pos+1, os.length()-pos));
	month = atoi(temp.c_str());
	
	year = atoi(os.c_str());
}

Date& Date::operator+=(const int& days)
{
	day += days;
	if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if(day > 31)
		{
			month += 1;
			day -= 31;
		}
	}
	else if(month == 4 || month == 6 || month == 9 || month == 11)
	{
		if(day > 30)
		{
			month += 1;
			day -= 30;
		}
	}
	else if(month == 2)
	{
		if(day > 28)
		{
			month += 1;
			day -= 28;
		}
	}
	if(month == 13)
	{
		month = 1;
		year += 1;
	}
	return *this;
}

bool Date::operator>=(const Date& other)
{
	if(year >= other.year)
	{
		return true;
	}
	else
	{
		if(month >= other.month)
		{
			return true;
		}
		else
		{
			if(day >= other.day)
			{
				return true;
			}
		}
	}
	return false;
}

void Date::getCurrentDate() 
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    day = timeinfo->tm_mday+1;
    month = timeinfo->tm_mon+1;
    year = timeinfo->tm_year+1900;
}

void Date::setDeadline(std::string date)
{
    std::string tmp;
    std::stringstream stream(date);
    int i = 0;
    while (getline(stream, tmp, '/')) 
    {
        switch (i) 
        {
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

std::string Date::toString()
{
	std::ostringstream os;
	os << day << "/" << month << "/" << year;
	std::string toReturn = os.str();
	return toReturn;
}

void Date::toDate(std::string os)
{
	int pos;
	std::string temp;
	
	pos = os.find_first_of("/");
	temp = (os.substr(0, pos));
	os = (os.substr(pos+1, os.length()-pos));
	day = atoi(temp.c_str());
	
	pos = os.find_first_of("/");
	temp = (os.substr(0, pos));
	os = (os.substr(pos+1, os.length()-pos));
	month = atoi(temp.c_str());
	
	year = atoi(temp.c_str());
}


