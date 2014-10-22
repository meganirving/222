#include "Classes.h"

void User::Display(bool& signedIn, std::string& input)
{
	std::cout << "I AM USER" << std::endl;
}

Author::Author(const std::string& name, const std::string& pass, const std::string& myemail)
{
	username = name;
	password = pass;
	email = myemail;
}

void Author::Display(bool& signedIn, std::string& input)
{
	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'S'ubmit work                   |" << std::endl;
	std::cout << "|   'E'mails                        |" << std::endl;
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "Q")
	{
		signedIn = false;
	}
}

Reviewer::Reviewer(const std::string& name, const std::string& pass, const std::string& myemail)
{
	username = name;
	password = pass;
	email = myemail;
}

void Reviewer::Display(bool& signedIn, std::string& input)
{
	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'R'eview work                   |" << std::endl;
	std::cout << "|   'S'et prefences                 |" << std::endl;
	std::cout << "|   'E'mails                        |" << std::endl;
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "Q")
	{
		signedIn = false;
	}
}

Admin::Admin(const std::string& name, const std::string& pass, const std::string& myemail)
{
	username = name;
	password = pass;
	email = myemail;
}

void Admin::Display(bool& signedIn, std::string& input)
{
	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'M'anage Users                  |" << std::endl;
	std::cout << "|   'S'et deadlines            	  |" << std::endl;
	std::cout << "|   'E'mails                        |" << std::endl;
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "M")
	{
	
	}
	else if(input == "Q")
	{
		signedIn = false;
	}
}
