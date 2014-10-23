#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <fstream>
#include <sstream>
#include <list>
#include "sharedglobals.h"

std::string check_User(const std::string&, const std::string&, const std::string&);
std::string get_Users(int&);
char* ItoA(int);

int main()
{
	srand(time(NULL));
	sf::Clock clock;
	
	sf::TcpListener listener;

		// bind the listener to a port
	if (listener.listen(1339) != sf::Socket::Done)
	{
	    // could not bind to port
	}
	
		// list of connected clients
	std::list<sf::TcpSocket*> clients;	
	
		// create a selector
	sf::SocketSelector selector;
	
		// add the listener to the selector
	selector.add(listener);
	
		//Setup info being sent through packet
	std::string ID;							// unique ID for server reference 
	PacketType Ptype;						// Packet type
	std::string username;					// username for an account
	std::string password;					// password for an account
	std::string email = "x";						// email address for an account
	std::string input;						// input command (server needs to know aswell)
	std::string userlevel = "-1";			// works out type of user e.g. Author, Admin
	
	while(1)
	{
			// possible memory leak because of all these
			// client sockets that we've been allocating
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
					// the listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				
					// accept a new connection
				if (listener.accept(*client) != sf::Socket::Done)
				{
				   		// no new connection, delete the socket
				    delete client;
				}
				else
				{
						// client successfully connected to socket
						// add the new client to the clients list
					clients.push_back(client);
					
						// add the new client to the selector
						// so that we will be notified when he
						// sends something
					selector.add(*client);
				}
			}
			else
			{
					// the listener socket is not ready
					// test all other sockets (the clients)
				for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
						//Set client to current iterator(i.e. socket)
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{ 
						// if we don't send packets we will need 
						// a size_t variable
						//std::size_t recieved = 0;
						
							// Receive packet from client
						sf::Packet loginPacket;
						sf::Packet serverReply;
							//basically a file.good() function for sockets
						if(client.receive(loginPacket) == sf::Socket::Done)
						{
								// Extract the variables contained in the packet
							
							loginPacket >> ID >> Ptype;
							switch(Ptype)
							{
								case 0: 
									loginPacket >> username >> password;
									std::cout << ID <<  " LOGIN REQUEST: " << username << ", " << password << std::endl; 
									userlevel = check_User(username, password, email);
										// send data to reply packet
									serverReply << Ptype << userlevel;
										// send reply
									client.send(serverReply);
									break;
								case 1:
									loginPacket >> username >> password >> email;
									std::cout << ID <<  " SIGN-UP REQUEST: " << username << " " << password << " " << email<< std::endl; 
						//make sure designated username doesn't already exist
									userlevel = check_User(username, password, email);
									if(userlevel == "NON-EXISTING")
									{
											//make sure file exists
										std::ofstream fout;
										std::ifstream fin("Users.txt");
										if(!fin.good())
										{
											fin.close();
											fout.open("Users.txt");
											fout << username << "," << password << "," << email << std::endl;
										}
										else
										{	
											fin.close();
											fout.open("Users.txt", std::ios::app);
											fout << username << "," << password << "," << email << std::endl;
										}
										fout.close();
									}
										// send data to reply packet
									serverReply << Ptype << userlevel;
										// send reply
									client.send(serverReply);
									break;
								case 2:
										std::cout << ID <<  " MANAGE USERS REQUEST" << std::endl; 
										std::string users;
										int count = 0;
										users = get_Users(count);
										
										serverReply << Ptype << count << users;
										
										client.send(serverReply);
									break;
							}
						}
					}
				}
			}
		}
	}
    return 0;
}

using namespace std;


	//just ignore this for now was creating my own itoa function just incase
char* ItoA(int num)
{
	std::string temp = "";
	std::ostringstream os;
	os << num;
	temp.append(os.str());

	char* converted = new char[temp.length() + 1];
	
	strcpy(converted, temp.c_str());
	return converted;
}

	//Finds what level of access the user should have
string check_User(const string& username, const string& password, const string& email)
{
	char tempUser[17];		//max username length = 16 + \0
	char tempPass[21];		//max password length = 20 + \0
	char tempEmail[254];	//http://stackoverflow.com/questions/386294/what-is-the-maximum-length-of-a-valid-email-address
	
		//check admin
	ifstream fin("PC_Chair.txt");
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout("PC_Chair.txt", ios::out);
		fout.close();
	}
	else			//search through
	{
		if(fin.good())	//if instead of while since theres only one admin
		{
			fin.getline(tempUser, 256, ',');
			fin.getline(tempPass, 256, ',');
			fin.getline(tempEmail, 256, '\n');
			
			if(!strcmp(tempUser, username.c_str()))
			{
				if(!strcmp(tempPass, password.c_str()))
				{
					return "ADMIN";
				}
				return "BAD-PASS";	
			}
			else
			{
				if(!strcmp(tempEmail, email.c_str()))
				{
					return "EXISTING-EMAIL";
				}
			}
		}
	}
	fin.close();
		//check Reviewers
	fin.open("Reviewers.txt", ios::app);
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout("Reviewers.txt", ios::out);
		fout.close();
	}
	else			//search through
	{
		while(!fin.eof())
		{
			fin.getline(tempUser, 256, ',');
			fin.getline(tempPass, 256, ',');
			fin.getline(tempEmail, 256, '\n');
			if(!strcmp(tempUser, username.c_str()))
			{
				if(!strcmp(tempPass, password.c_str()))
				{
					return "REVIEWER";
				}
				return "BAD-PASS";	
			}
			else
			{
				if(!strcmp(tempEmail, email.c_str()))
				{
					return "EXISTING-EMAIL";
				}
			}
		}
	}
	fin.close();
	
		//check Authors
	fin.open("Authors.txt", ios::app);
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout("Authors.txt", ios::out);
		fout.close();
	}
	else			//search through
	{
		while(!fin.eof())
		{
			fin.getline(tempUser, 256, ',');
			fin.getline(tempPass, 256, ',');
			fin.getline(tempEmail, 256, '\n');
			if(!strcmp(tempUser, username.c_str()))
			{
				if(!strcmp(tempPass, password.c_str()))
				{
					return "AUTHOR";
				}
				return "BAD-PASS";	
			}
			else
			{
				if(!strcmp(tempEmail, email.c_str()))
				{
					return "EXISTING-EMAIL";
				}
			}
		}
	}
	fin.close();
	
		//check Users
	fin.open("Users.txt", ios::app);
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout("Users.txt", ios::out);
		fout.close();
	}
	else			//search through
	{
		while(!fin.eof())
		{
			fin.getline(tempUser, 256, ',');
			fin.getline(tempPass, 256, ',');
			fin.getline(tempEmail, 256, '\n');
			if(!strcmp(tempUser, username.c_str()))
			{
				if(!strcmp(tempPass, password.c_str()))
				{
					return "UNAPPROVED";
				}
				return "NON-EXISTING";	
			}
			else
			{
				if(!strcmp(tempEmail, email.c_str()))
				{
					return "EXISTING-EMAIL";
				}
			}
		}
	}
	fin.close();

	return "NON-EXISTING";
}

std::string get_Users(int& counter)
{
	ifstream fin;
	char tempLine[256];
	char end_of_user = '~';
	std::string users = "";
		//check Authors
	fin.open("Users.txt", ios::in);
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout("Users.txt", ios::out);
		fout.close();
	}
	else			//search through
	{
		while(fin.good())
		{
			fin.getline(tempLine, 256, '\n');
			if(fin.good())
			{
				users.append(tempLine);
				users += end_of_user;
				counter++;
			}
		}
	}
	fin.close();
	return users;
}