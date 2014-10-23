#include "Classes.h"

void User::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
{
	std::cout << "I AM USER" << std::endl;
}

Author::Author(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Author::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
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

Reviewer::Reviewer(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Reviewer::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
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

Admin::Admin(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Admin::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
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
		ManageUsers(socket);
	}
	else if(input == "Q")
	{
		signedIn = false;
	}
}

void Admin::ManageUsers(sf::TcpSocket& socket)
{
	std::string users;
	int userCount;
	
	PacketType Ptype = GET_USERS;

		//Create Packet
	sf::Packet loginPacket;
	
		//Send data to packet
	loginPacket << id << Ptype;
	
		//Send packet to server
	socket.send(loginPacket);	
	
	sf::Packet serverReply;
			
	if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
	{
			//push data from server reply into variables
		serverReply >> Ptype;
		if(Ptype == 2)
		{
			serverReply >> userCount >> users;
			std::cout << "RECIEVED: " << userCount << " " << users << std::endl;
		}
		else
		{
			std::cout << "EXPECTED: GET_USERS RECIEVED: " << Ptype << std::endl;
		}
	}
	
	std::cout << "There are currently " << userCount << " user requests: " << std::endl;
	
	int pos = 0;
	for(int i = 0; i < userCount; i++)
	{
		pos = users.find_first_of("~");
		std::string tempUser = users.substr(0, pos);
		users = users.substr(pos+1, users.length()-pos);
		std::cout << tempUser << std::endl;
		//std::cout << "users: " << users << pos << std::endl;
	}
}
