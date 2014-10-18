#include "Classes.h"

void User::Display(bool& signedIn, std::string& input)
{
	std::cout << "I AM USER" << std::endl;
}

Author::Author(const std::string& name, const std::string& pass)
{
	username = name;
	password = pass;
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

Reviewer::Reviewer(const std::string& name, const std::string& pass)
{
	username = name;
	password = pass;
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
