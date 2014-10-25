#include "Reviewer.h"

Reviewer::Reviewer(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Reviewer::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
{
	loadNotifs(socket);

	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'R'eview works                  |" << std::endl;
	std::cout << "|   'S'et prefences                 |" << std::endl;
	std::cout << "|   'B'id on papers                 |" << std::endl;
	std::cout << "|   'C'omment on reviews            |" << std::endl;
	notifMenu();
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "Q")
	{
		signedIn = false;
	}
	else if(input == "N")
	{
		displayNotifs(socket);
	}
	else if (input == "R")
	{
		reviewMenu(socket);
	}
	else if (input == "C")
	{
		commentMenu(socket);
	}
}

// get the review form and submit the review form
void Reviewer::reviewMenu(sf::TcpSocket& socket) 
{
	// just testing
	papernames.push_back("test paper");
	papernames.push_back("other test");
	
	std::vector<std::string>::iterator itr = papernames.begin();
	
	// display menu until exit
	char input = 'x';
	while (input != 'B')
	{
		// display menu
		std::cout << "\n|\t" << *itr << "\t\t|" << std::endl;
		std::cout << "|\t\t\t\t|" << std::endl;
		// options for this paper
		std::cout << "|\t'G'et Form\t\t|" << std::endl;
		std::cout << "|\t'S'ubmit Review\t\t|" << std::endl;
		std::cout << "|\t\t\t\t|" << std::endl;
		// change paper/go back
		if (papernames.size() > 1)
		{
			std::cout << "|\t'N'ext Paper\t\t|" << std::endl;
			std::cout << "|\t'P'rev Paper\t\t|" << std::endl;
		}
		std::cout << "|\t'B'ack\t\t\t|" << std::endl;

		// get input
		std::cin >> input;

		// deal with input
		switch(input)
		{
			case 'G': // get form
			{
				std::cout << "\nDownloading review form..." << std::endl;
				generateForm(*itr);
				std::cout << "Review form downloaded!\n" << std::endl;
				break;
			}
			case 'S': // submit form
			{
				submitReview(socket, id, username, *itr);
				break;
			}
			case 'N': // view next paper
			{
				std::vector<std::string>::iterator temp = itr;
				temp++;
				if (temp != papernames.end())
				{
					itr = temp;
				}
				break;
			}
			case 'P': // view prev paper
			{
				if (itr != papernames.begin())
				{
					itr--;
				}
				break;
			}
		}
	}
}

