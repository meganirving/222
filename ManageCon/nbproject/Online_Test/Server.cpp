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
#include <vector>
#include "sharedglobals.h"

void loadNotifs(sf::TcpSocket&, sf::Packet&);
std::string check_User(const std::string&, const std::string&, const std::string&);
std::string check_File(const std::string&, const std::string&, const std::string&, const std::string&);
std::string get_Users(int&);
char* ItoA(int);

int main()
{
	srand(time(NULL));
	sf::Clock clock;
	
	sf::TcpListener listener;

		// bind the listener to a port
	if (listener.listen(1338) != sf::Socket::Done)
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
	std::string userType;					// type of user to become
	std::string email = "x";				// email address for an account
	std::string input;						// input command (server needs 
											// to know aswell)
	std::string userlevel = "-1";			// works out type of user 
											// e.g. Author, Admin
	
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
								{
									loginPacket >> username >> password;
									std::cout << ID <<  " LOGIN REQUEST: " << username << ", " << password << std::endl; 
									userlevel = check_User(username, password, email);
										// send data to reply packet
									serverReply << Ptype << userlevel;
										// send reply
									client.send(serverReply);
									break;
								}
								case 1:
								{
									loginPacket >> username >> password >> userType >> email;
									std::cout << ID <<  " SIGN-UP REQUEST: " << username << " " << password << " " << userType << " " << email << std::endl; 
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
											fout << username << "," << password << "," << userType << "," << email << std::endl;
										}
										else
										{	
											fin.close();
											fout.open("Users.txt", std::ios::app);
											fout << username << "," << password << "," << userType << "," << email << std::endl;
										}
										fout.close();
									}
										// send data to reply packet
									serverReply << Ptype << userlevel;
										// send reply
									client.send(serverReply);
									break;
								}
								case 2:	//GET AMOUNT OF USERS
								{
									std::cout << ID <<  " MANAGE USERS REQUEST" << std::endl; 
									std::string users;
									int count = 0;
									users = get_Users(count);
									
									serverReply << Ptype << count << users;
									
									client.send(serverReply);
									break;
								}
								case 3:	//ACCEPT USER
								{
									std::string userToAccept;
									std::string tempUser;
									std::string userToDel;
									std::string tempLevel;
									char tempLine[256];
									std::string tempToAdd;
									std::vector<std::string> tempFileData;
									int pos;
									std::ifstream fin;
									std::ofstream fout;
									
									loginPacket >> userToAccept;
									
									std::cout << ID <<  " ADD USER REQUEST " << userToAccept << std::endl; 
									
									fin.open("Users.txt");
									while(fin.good())
									{
										fin.getline(tempLine, 256, ',');
										tempUser = tempLine;
										if(tempUser == userToAccept)
										{
											fin.getline(tempLine, 256, ',');
											tempToAdd = tempLine;
											userToAccept += ',';
											userToAccept.append(tempToAdd);
											fin.getline(tempLine, 256, ',');
											tempLevel = tempLine;
											userToAccept += ',';
											fin.getline(tempLine, 256, '\n');
											tempToAdd = tempLine;
											userToAccept.append(tempToAdd);
											if(tempLevel == "AUTHOR")
											{
												
												fout.open("Authors.txt", std::ios::app);
												if(fout.good())
												{
													fout << userToAccept << std::endl;
												}
												fout.close();
											}
											else if(tempLevel == "REVIEWER")
											{
												fout.open("Reviewers.txt", std::ios::app);
												if(fout.good())
												{
													fout << userToAccept << std::endl;
												}
												fout.close();
											}
										}
										else
										{
											fin.getline(tempLine, 256, '\n');
										}
									}
									fin.close();
									
									fin.open("Users.txt");
									while(fin.good())
									{
										fin.getline(tempLine, 256, '\n');
										tempUser = tempLine;
										tempFileData.push_back(tempUser);
									}
									for(int i = 0; i < tempFileData.size(); i++)
									{
										pos = tempFileData[i].find_first_of(',');
										tempUser = (tempFileData[i].substr(0, pos));
										pos = userToAccept.find_first_of(',');
										userToDel = (userToAccept.substr(0, pos));
										if(tempUser == userToDel)
										{
											tempFileData.erase(tempFileData.begin()+i);
											i = tempFileData.size();
										}
									}
									fin.close();
									fout.open("Users.txt");
									for(int i = 0; i < tempFileData.size(); i++)
									{
										fout << tempFileData[i];
										if(i < tempFileData.size()-1)
										{
											fout << '\n';
										}
									}
									fout.close();
									
									serverReply << Ptype;
									
									client.send(serverReply);
								
									break;
								}
								case 4:	//SUBMIT PDF FILE
								{
									int size = 0, max_size = 0;
									std::string filename;
									std::string keywords[5];
									std::size_t received = 0;

									char *Buffer = new char[4096];

									loginPacket >> filename >> keywords[0] >> keywords[1] >> keywords[2] >> keywords[3] >> keywords[4] >> size;
									std::cout << ID << " FILE SUBMISSION: " << filename << " has a file size of: " << size <<std::endl;
									//We will have to create a directory on the pc we use for the presentation
									// the commented out strings are specific to my home pc

									//std::string keyname = "/home/tyson/Documents/CSCI222/Submissions/" + filename + "_keywords.txt";
									//std::string file_out = "/home/tyson/Documents/CSCI222/Submissions/"+ filename;
									
									std::string keyname = filename + "_keywords.txt";	
									std::ofstream outfile;
									std::ofstream keyfile;

									outfile.open(filename.c_str(), std::ofstream::binary);
									keyfile.open(keyname.c_str(), std::ios::app);
										
									for(int i = 0; i < 5; i++)
									{
										keyfile << keywords[i] << std::endl;
									}
									while(size > max_size)
									{
										//Recieve raw Data
										client.receive(Buffer, sizeof(Buffer), received);
										//Output to designated file
										outfile.write(Buffer, sizeof(Buffer));
										//Keep track of file size so we know when file
										//is completely transfered
										max_size += sizeof(Buffer);
									}
								}
								case 5:	//NOTIFIES
								{
									loadNotifs(client, loginPacket);
								}
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

// loads notifs from a text file
void loadNotifs(sf::TcpSocket& client, sf::Packet& NotifPacket)
{
	std::string username;
	std::string notifs;
	int count;
	PacketType pType = GET_NOTIFICATION;

	NotifPacket >> username;
									
	// get the filename
	std::string fname = username;
	fname.append("_notifs.txt");
	
	// open the file
	std::ifstream ifile;
	ifile.open(fname.c_str(), std::ifstream::in);
	
	// if it's open
	if (ifile.is_open())
	{
		notifs = "";
		std::string next = "";
		count = 0;
	
		// check it's good
		while (!ifile.eof())
		{
			// get the next line and add a delim
			std::getline(ifile, next);
			notifs.append(next);
			notifs.append("~");
			count++;
		}
		count--;
		
	}
	
	// send back to client
	ifile.close();
	sf::Packet serverReply;
	serverReply << pType << count << notifs;								
	client.send(serverReply);
}


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
		//check admin
	std::string returnvalue = "NON-EXISTING";
	std::string fname = "PC_Chair.txt";
	returnvalue = check_File(fname, username, password, email);
		//check Reviewers
	if(returnvalue == "NON-EXISTING")
	{
		fname = "Reviewers.txt";
		returnvalue = check_File(fname, username, password, email);
	}
	if(returnvalue == "NON-EXISTING")
	{
			//check Authors
		fname = "Authors.txt";
		returnvalue = check_File(fname, username, password, email);
	}
	if(returnvalue == "NON-EXISTING")
	{
			//check Users
		fname = "Users.txt";
		returnvalue = check_File(fname, username, password, email);
	}
	return returnvalue;
}

std::string check_File(const std::string& fname, const std::string& username, const std::string& password, const std::string& email)
{
	char tempUser[17];		//max username length = 16 + \0
	char tempPass[21];		//max password length = 20 + \0
	char tempEmail[254];
	ifstream fin(fname.c_str());
	if(!fin.good())	//make sure file exists, otherwise create one
	{
		fin.close();
		ofstream fout(fname.c_str(), ios::out);
		fout.close();
	}
	else			//search through
	{
		while(fin.good())	//if instead of while since theres only one admin
		{
			fin.getline(tempUser, 256, ',');
			fin.getline(tempPass, 256, ',');
			fin.getline(tempEmail, 256, '\n');
			
			if(!strcmp(tempUser, username.c_str()))
			{
				if(!strcmp(tempPass, password.c_str()))
				{
					if(fname == "PC_Chair.txt")
					{
						return "ADMIN";
					}
					else if(fname == "Reviewers.txt")
					{
						return "REVIWER";
					}
					else if(fname == "Authors.txt")
					{
						return "AUTHOR";
					}
					else if(fname == "Users.txt")
					{
						return "USER";
					}
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
	return "NON-EXISTING";
}

std::string get_Users(int& counter)
{
	ifstream fin;
	char tempLine[1024];
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
