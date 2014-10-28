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
#include "Date.h"

void getPhase(sf::TcpSocket&, const PacketType&);
void getAllNews(sf::TcpSocket&);
void saveNews(sf::Packet&);
void submitReview(sf::TcpSocket&, sf::Packet&);
void getAllReviews(sf::TcpSocket&, sf::Packet&);
void addComment(sf::TcpSocket&, sf::Packet&);
void getDeadline(sf::TcpSocket&, const PacketType&);
void checkDeadline(sf::TcpSocket&, const PacketType&);
void setDeadline(sf::TcpSocket&, const PacketType&, sf::Packet&);
void loadNotifs(sf::TcpSocket&, sf::Packet&);
void saveNotifs(sf::TcpSocket&, sf::Packet&);
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
	std::string userType;					// type of user to become
	std::string email = "x";				// email address for an account
	std::string input;						// input command (server needs 
											// to know aswell)
	std::string userlevel = "-1";			// works out type of user 
											// e.g. Author, Admin
	
	//		SYSTEM
	//
	//	Send automatic emails
	//	Send notifications
	//	Hand out papers based on keywords and bids
	//	Keep track of phases				
	//-------------------------------------------------------------------------
	//										Admin opens submission phase
	//										Date DD/MM/YYYY
	//		-- submission phase			0	23/10/2014		
	//											admin opens up submission phase
	//		-- bidding phase			1	30/10/2014
	//											submission end deadline reached
	//											bidding phase
	//		-- reviewing phase			2	06/11/2014
	//											close bidding phase
	//											begin reviewing phase	
	//		-- review inception phase	3	13/11/2014
	//											close reviewing phase
	//											reviewers can respond to reviews
	//		-- rebuttal phase			4	
	//											close reviewing reponses
	//											authors can respond to reviews
	//		-- Admin accept phase		5
	//											close responses
	//											accept paper
	//		-- Conference				6		
	//											if paper accepted present at
	//											conference 
	//-------------------------------------------------------------------------
	//	Update newsfeed
	//
	
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
										//std::cout << "REDCIEVED PACKET" << std::endl;
									int size = 0, size2 = 0, max_size = 0;
									std::string username;
									std::string filename;
									std::string filename2;
									std::string keywords[5];
									std::size_t received = 0;

									char *Buffer = new char[4096];
loginPacket >> filename >> filename2 >>username >> keywords[0] >> keywords[1] >> keywords[2] >> keywords[3] >> keywords[4] >> size >> size2;
									
									sf::Packet checker;
									checker << Ptype;
									
									client.send(checker);
									
									std::cout << ID << " FILE SUBMISSION: " << filename << " has a file size of: " << size <<std::endl;
									std::cout << ID << " FILE SUBMISSION: " << filename2 << " has a file size of: " << size2 <<std::endl;
								
									//We will have to create a directory on the pc we use for the presentation
									// the commented out strings are specific to my home pc
									std::ofstream username_file;
									std::ofstream filename_database;
									std::string user_file = username + "_submissions.txt";
									
									username_file.open(username.c_str(), std::ios::app);
									filename_database.open("filenames.txt", std::ios::app);	

									username_file << filename << std::endl;
									
									std::string buffer = filename;


									buffer.erase(buffer.end()-4, buffer.end());
									
									
									std::string keyname = buffer + "_keywords.txt";	
									std::ofstream outfile;
									std::ofstream keyfile;
									
									outfile.open(filename.c_str(), std::ofstream::binary);
									keyfile.open(keyname.c_str(), std::ios::app);
										
									for(int i = 0; i < 5; i++)
									{
										keyfile << keywords[i] << std::endl;
									}
									std::string test;
									sf::Packet mytemp;
									Buffer = new char[4096];
									while(size > max_size)
									{
										if(client.receive(mytemp) == sf::Socket::Done)
										{
											mytemp >> test;
											strcpy(Buffer, test.c_str());
											mytemp.clear();
											//Recieve raw Data
											//Output to designated file
											outfile.write(Buffer, sizeof(Buffer));
											//Keep track of file size so we know when file
											//is completely transfered
											max_size += sizeof(Buffer);
										}
									}
									std::cout << "COMPLETED FIRST FILE" << std::endl;
									outfile.close();
									
									checker.clear();
									checker << Ptype;
									
									client.send(checker);
									
									
									max_size = 0;
									outfile.open(filename2.c_str(), std::ofstream::binary);
									
									test.clear();
									while(size2 > max_size)
									{
										if(client.receive(mytemp) == sf::Socket::Done)
										{
											mytemp >> test;
											mytemp.clear();
											//Recieve raw Data
											//Output to designated file
											outfile.write(test.c_str(), sizeof(test.c_str()));
											//Keep track of file size so we know when file
											//is completely transfered
											max_size += sizeof(test.c_str());
										}
									}
									outfile.write(test.c_str(), sizeof(test));
									std::cout << "COMPLETED SECOND FILE" << std::endl;
									outfile.close();
									
									checker.clear();
									checker << Ptype;
									
									client.send(checker);
									
									filename_database << filename << " " << username << std::endl;
									//std::cout << "BREAK OUT" << std::endl;
									break;
								}
								case 5:	//NOTIFIES
								{
									std::cout << ID << " LOAD NOTIFICATIONS REQUEST" <<std::endl;
									loadNotifs(client, loginPacket);
									break;
								}
								case 6: //GET DEADLINES
								{
									std::cout << ID << " GET DEADLINES REQUEST" <<std::endl;
									getDeadline(client, Ptype);
									break;
								}
								case 7: //SET DEADLINES
								{
									std::cout << ID;
									setDeadline(client, Ptype, loginPacket);
									break;
								}
								case 8: // SAVE NOTIFICATIONS
								{
									saveNotifs(client, loginPacket);
									break;
								}
								case 9: // SAVE COMMENT
								{
									addComment(client, loginPacket);
									break;
								}
								case 10: // SAVE REVIEW
								{
									submitReview(client, loginPacket);
									break;
								}
								case 11: // GET ALL REVIEWS
								{
									getAllReviews(client, loginPacket);
									break;
								}
								case 12: // GET THE NEWSFEED
								{
									getAllNews(client);
									break;
								}
								case 13: // SAVE THE NEWSFEED
								{
									saveNews(loginPacket);
									break;
								}
								case 14: // GET THE CURRENT PHASE
								{
									getPhase(client, Ptype);
									break;
								}
								case 15: //Recieve new filenames.txt
								{
									std::string buffer;
									
									loginPacket << buffer;
									
									std::ofstream ofile;
									ofile.open("filenames.txt", std::ios::out);
									ofile << buffer;
									ofile.close();
									break;
								}
								case 16: // CHECK_DEADLINE
								{
									std::cout << "CHECK DEADLINE" << std::endl;
									checkDeadline(client, Ptype);
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

// server:
void getAllNews(sf::TcpSocket& client)
{
	// open file
	std::string news = "";
	std::string next = "";
	std::ifstream ifile;
	ifile.open("newsfeed.txt", std::ios::in);
	
	// if file exists
	if (ifile.is_open())
	{
		// get the file out
		while(!ifile.eof())
		{
			getline(ifile, next);
			if (!ifile.eof())
			{
				news.append(next);
				news.append("\n");
			}
		}
		
		// if file is empty (aka: no *s)
		int pos = news.find('*');
		if (pos == std::string::npos)
		{
			// create new news
			news = "No news is good news*\n";
			
			// write to file
			std::ofstream ofile;
			ofile.open("newsfeed.txt", std::ios::out);
			ofile << news;
			ofile.close();
		}
	}
	// no news yet, so add news and..return that?
	else
	{
		std::ofstream ofile;
		ofile.open("newsfeed.txt", std::ios::out);
		news = "No news is good news*\n";
		ofile << news;
		ofile.close();
	}

	// close input file
	ifile.close();
	
	// send to client
	sf::Packet replyPacket;
	PacketType pType = GET_NEWS;
	replyPacket << pType << news;
	client.send(replyPacket);
}
void saveNews(sf::Packet& newsPacket)
{
	// get the news string
	std::string news;
	newsPacket >> news;

	// write to the file and close it
	std::ofstream ofile;
	ofile.open("newsfeed.txt", std::ios::out);
	ofile << news << std::endl;
	ofile.close();
}

// change current deadline settings
void setDeadline(sf::TcpSocket& client, const PacketType& Ptype, sf::Packet& DeadlinePacket)
{
	std::string input;
	int phase;
	std::string deadLineDate;
	Date tempDate(deadLineDate);
	
	DeadlinePacket >> phase >> deadLineDate >> input;
	std::cout << " SET DEADLINES REQUEST: " << phase << " " << tempDate << std::endl;
	if(input == "P")
	{
		tempDate.getCurrentDate();
		tempDate += 7;
		deadLineDate = tempDate.toString();
	}
	std::ofstream fout;
	fout.open("Settings.txt");
	fout << phase << " " << deadLineDate;
	fout.close();
	
	sf::Packet serverReply;
	serverReply << Ptype;								
	client.send(serverReply);
}

// load Deadlines from a text file
void getDeadline(sf::TcpSocket& client, const PacketType& Ptype)
{
	std::ifstream fin;
	int phase;
	std::string deadLineDate;
	fin.open("Settings.txt");
	
	if(fin.good())
	{
		fin >> phase;
		fin >> deadLineDate;
		fin.close();
	}
	else
	{
		fin.close();
		phase = -1;
		deadLineDate = "00/00/0000";
		std::ofstream fout;
		fout.open("Settings.txt");
		fout << phase << " " << deadLineDate;
		fout.close();
	}
	
	sf::Packet serverReply;
	serverReply << Ptype << phase << deadLineDate;								
	client.send(serverReply);
}

void checkDeadline(sf::TcpSocket& client, const PacketType& pType)
{
	bool updated = false;
	std::ifstream fin;
	int phase;
	std::string deadLineDate;
	fin.open("Settings.txt");
	
	if(fin.good())
	{
		fin >> phase;
		fin >> deadLineDate;
		fin.close();
		if(deadLineDate != "00/00/0000")
		{
			Date tempDate;
			tempDate.getCurrentDate();
			Date testDate(deadLineDate);
			if(tempDate >= testDate)
			{
				std::cout << tempDate << " >= " << testDate << std::endl;
				phase++;
				if(phase > 6)
				{
					phase = 0;
				}
				std::cout << "DEADLINE REACHED MOVING TO NEXT PHASE" << std::endl;
				
				tempDate += 7;
				deadLineDate = tempDate.toString();
				std::ofstream fout;
				fout.open("Settings.txt");
				fout << phase << " " << deadLineDate;
				fout.close();
				updated = true;
			}
		}
	}
	else
	{
		fin.close();
		phase = -1;
		deadLineDate = "00/00/0000";
		std::ofstream fout;
		fout.open("Settings.txt");
		fout << phase << " " << deadLineDate;
		fout.close();
	}
	sf::Packet serverReply;
	serverReply << pType << phase << updated;						
	client.send(serverReply);
}	

// load the phase from a text file
void getPhase(sf::TcpSocket& client, const PacketType& Ptype)
{
	std::ifstream fin;
	int phase;
	std::string deadLineDate;
	fin.open("Settings.txt");
	
	if(fin.good())
	{
		fin >> phase;
		fin >> deadLineDate;
		fin.close();
	}
	else
	{
		fin.close();
		phase = -1;
		deadLineDate = "00/00/0000";
		std::ofstream fout;
		fout.open("Settings.txt");
		fout << phase << " " << deadLineDate;
		fout.close();
	}
	
	sf::Packet serverReply;
	serverReply << Ptype << phase;						
	client.send(serverReply);
}

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

// save notifs to a text file
void saveNotifs(sf::TcpSocket& client, sf::Packet& notifPacket)
{
	// get the filename and the notifs
	std::string fname;
	std::string notifs;
	notifPacket << fname << notifs;
	
	// open file
	std::ofstream ofile;
	ofile.open(fname.c_str(), std::ios::out);
	
	// write data and close file
	ofile << notifs;
	ofile.close();
}

// saves a review to a text file
void submitReview(sf::TcpSocket& client, sf::Packet& reviewPacket)
{
	// get filename
	std::string fname;
	std::string username;
	std::string review;
	sf::Packet packet;
	
	reviewPacket >> fname >> username >> review;
	
	std::ifstream ifile;
	ifile.open(fname.c_str(), std::ios::in);
	
	// file exists
	if(ifile.is_open())
	{
		std::string allreviews;
		std::string next;
		
		// get all the data
		while (!ifile.eof())
		{
			getline(ifile, next);
			if (!ifile.eof())
			{
				allreviews.append(next);
				allreviews.append("\n");
			}
		}
		ifile.close();
		
		// look for matching username
		int found = allreviews.find(username);
		// if found
		if (found != std::string::npos)
		{
			int start = allreviews.find_last_of("~", found);
			std::string before = allreviews.substr(0, start); // get all the reviews before this one's ~
			int end = allreviews.find_first_of("~", found); // get the end of this review
			std::string after = allreviews.substr(end+1, allreviews.length()-end); // get all reviews after this one's ~
			
			// write to file
			std::string newreviews = before + review + after;
			std::ofstream ofile;
			ofile.open(fname.c_str(), std::ios::out);
			ofile << newreviews;
			ofile.close();
		}
		// if not found
		else
		{
			std::ofstream ofile;
			ofile.open(fname.c_str(), std::ios::app);
			ofile << "\n" << review;
			ofile.close();
		}
	}
	// file doesn't exist
	else
	{
		// just write the file
		ifile.close();
		std::ofstream ofile;
		ofile.open(fname.c_str(), std::ios::out);
		ofile << review;
		ofile.close();
	}
}

// gets all reviews from a text file
void getAllReviews(sf::TcpSocket& client, sf::Packet& reviewPacket)
{
	// get filename
	std::string fname;
	reviewPacket >> fname;
	
	// open file
	std::ifstream ifile;
	ifile.open(fname.c_str(), std::ios::in);
	
	std::string next = "";
	std::string reviews = "";
	
	if (ifile.is_open())
	{
		// get content into a string
		while (!ifile.eof())
		{
			getline(ifile, next);
			if (!ifile.eof())
			{
				reviews.append(next);
				reviews.append("\n");
			}
		}
	}
	else
	{
		std::cout << "error: could not open " << fname << std::endl;
	}
	
	// send string to client
	sf::Packet replyPacket;
	PacketType pType = GET_REVIEWS;
	replyPacket << pType << reviews;
	client.send(replyPacket);
}

// saves a comment to the review text file
void addComment(sf::TcpSocket& client, sf::Packet& commentPacket)
{
	std::string fname;
	std::string reviews;
	
	// get filename and reviews out of string
	commentPacket >> fname >> reviews;
	
	// write reviews to file
	std::ofstream ofile;
	ofile.open(fname.c_str(), std::ios::out);
	ofile << reviews;
	ofile.close();
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
						return "REVIEWER";
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
