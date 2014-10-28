#include "Admin.h"
#include "Colours.h"

Admin::Admin(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Admin::paperMenu(sf::TcpSocket& socket) 
{
	std::vector<std::string>::iterator itr = papernames.begin();
	std::vector<std::string>::iterator name_itr = usernames.begin();
	
	// display menu until go back
	char input = 'x';
	while (input != 'B')
	{
		// display paper name and options
		std::cout << "\n|\t\t" << *itr << "\t|" << std::endl;
		std::cout << "|\tAverage Score: " << avScoreOfPaper(socket, id, *itr) << "\t|" << std::endl;
		
		std::cout << "|\t'V'iew Reviews\t\t|" << std::endl;
		
		std::cout << "|\t\t\t\t|" << std::endl;
		std::cout << "|\t'A'ccept Paper\t\t|" << std::endl;
		std::cout << "|\t'R'eject Paper\t\t|" << std::endl;
		
		std::cout << "|\t\t\t\t|" << std::endl;
		std::cout << "|\t'P'rev Paper\t\t|" << std::endl;
		std::cout << "|\t'N'ext Paper\t\t|" << std::endl;
		std::cout << "|\t'B'ack\t\t\t|" << std::endl;
		
		// get input and deal with it
		std::cin >> input;
		switch(input)
		{
			case 'V': // view reviews of this paper
			{
				reviewCommentMenu(socket, *itr);
				break;
			}
			case 'N': // view next paper
			{
				std::vector<std::string>::iterator temp = itr;
				temp++;
				if (temp != papernames.end())
				{
					itr = temp;
					name_itr++;
				}
				break;
			}
			case 'P': // view prev paper
			{
				if (itr != papernames.begin())
				{
					itr--;
					name_itr--;
				}
				break;
			}
			case 'A': // accept paper
			{
				AcceptPapers(*itr, socket, *name_itr);
				break;	
			}
			case 'R':
			{
				RejectPapers(*itr, socket, *name_itr);
				break;
			}
		}
	}
}

void Admin::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
{
	loadNotifs(socket);
	testDeadline(socket, id);
	
	// header
	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'M'anage Users                  |" << std::endl;
	
	// conditional
	if (currPhase == 5)
	{
		std::cout << "|   'A'ccept Papers                 |" << std::endl;
	}
	
	// footer
	std::cout << "|   'S'et Deadlines                 |" << std::endl;
	notifMenu();
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	
	// handle input
	std::cin >> input;
	if(input == "M")
	{
		ManageUsers(socket);
	}
	else if(input == "N")
	{
		displayNotifs(socket);
	}
	else if(input == "S")
	{
		set_Deadline(socket);
	}
	else if(input == "Q")
	{
		signedIn = false;
	}
	else if(input =="A")
	{
		ManagePapers(socket);
	}
}

void Admin::ManagePapers(sf::TcpSocket& socket)
{
	papernames.push_back("test paper");
	papernames.push_back("other test");

	std::ifstream filename_database;
	std::string username;
	std::string buffer;
	filename_database.open("filenames.txt");	
	
	while(!filename_database.eof())
	{
	
		filename_database >> buffer;
		getline(filename_database, username);
		if(!filename_database.eof())
		{
			papernames.push_back(buffer);	
			usernames.push_back(username);
			
		}		
	}
	
	paperMenu(socket);
}
void Admin::AcceptPapers(std::string file, sf::TcpSocket& socket, std::string username)
{
	file.erase(file.length()-5, 4);
	std::string news = username + "'s paper " + file + " has been added to the conference!\n";
	
	addNews(news, socket, id);
				
}
void Admin::RejectPapers(std::string file, sf::TcpSocket& socket, std::string username)
{
	std::string ID = "02030403";
	PacketType pType = OVERIDE_FILENAMES;
	std::vector<std::string>::iterator itr = papernames.begin();
	std::vector<std::string>::iterator name_itr = usernames.begin();
	std::string buffer;
	bool found = false;
	
	while(itr != papernames.end() && !found)
	{
		if(*itr == file)
		{
			found = true;
		}
		else
		{
			itr++;
			name_itr++;
		}
	}
	
	if(found)
	{
		papernames.erase(itr);
		usernames.erase(name_itr);
	}	
	itr = papernames.begin();
	name_itr = usernames.begin();
	
	while(itr != papernames.end())
	{
		buffer.append(*itr + " " + *name_itr + "\n");
		itr++;
		name_itr++;
	}
	
	sf::Packet packet;
	packet << ID << pType << buffer;
	
	socket.send(packet);	

}

void Admin::ManageUsers(sf::TcpSocket& socket)
{		
	std::string users;
	std::string levels;
	int userCount;
	std::string input = "x";
	char acceptUser = 'x';
	int usernumber = -1;
	
	
	while(input != "B")
	{
		getUsers(socket, users, levels, userCount);
		std::string myusers = users;
		std::string mylevels = levels;
	
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|            User Menu              |" << std::endl;
		std::cout << "|   '#' Accept user number          |" << std::endl;
		std::cout << "|   'B'ack                          |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		std::cout << "\n--> ";
		std::cin >> input;
		if(input != "B")
		{
			usernumber = atoi(input.c_str());
			if(usernumber <= userCount && usernumber > 0)
			{
				std::string designatedUser;
				std::string designatedLevel;
				int pos;
				for(int i = 0; i < usernumber; i++)
				{
					pos = myusers.find_first_of(",");
					designatedUser = (myusers.substr(0, pos));
					myusers = myusers.substr(pos+1, myusers.length()-pos);
					
					pos = mylevels.find_first_of(",");
					designatedLevel = (mylevels.substr(0, pos));
					mylevels = mylevels.substr(pos+1, mylevels.length()-pos);
				}
				
				
				PacketType Ptype = ACCEPT_USER;

					//Create Packet
				sf::Packet loginPacket;

					//Send data to packet
				loginPacket << id << Ptype << designatedUser;
				
					//Send packet to server
				socket.send(loginPacket);	

				sf::Packet serverReply;
	
				if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
				{
						//push data from server reply into variables
					serverReply >> Ptype;
					if(Ptype == 3)
					{
						std::cout << "User has been accepted" << std::endl;
					}
					else
					{
						std::cout << "EXPECTED: ACCEPT_USER RECIEVED: " << Ptype << std::endl;
					}
				}
			}
			else
			{
				std::cout << "Invalid user number" << std::endl;
			}
		}
	}
}

void Admin::getUsers(sf::TcpSocket& socket, std::string& tempUsers, std::string& tempLevels, int& userCount)
{
	std::string users;
	
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
		}
		else
		{
			std::cout << "EXPECTED: GET_USERS RECIEVED: " << Ptype << std::endl;
		}
	}
	
	std::cout << "There are currently " << userCount << " user requests: " << std::endl;
	
	
	tempUsers.clear();
	tempLevels.clear();
	int pos = 0;
	std::string tempUser;
	std::string tempPass;
	std::string tempLevel;
	std::string tempEmail;
	std::cout << "List of user request: " << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	for(int i = 0; i < userCount; i++)
	{
			//get user name
		pos = users.find_first_of(",");
		tempUser = (users.substr(0, pos));
		users = users.substr(pos+1, users.length()-pos);
		std::cout << "|- " << i+1 << ": " << tempUser;
			//get password
		pos = users.find_first_of(",");
		tempPass = users.substr(0, pos);
		users = users.substr(pos+1, users.length()-pos);
		std::cout << ", " << tempPass;
			//get request level
		pos = users.find_first_of(",");
		tempLevel = (users.substr(0, pos));
		users = users.substr(pos+1, users.length()-pos);
		std::cout << ", " << tempLevel;
			//get email
		pos = users.find_first_of("~");
		tempEmail = users.substr(0, pos);
		users = users.substr(pos+1, users.length()-pos);
		std::cout << ", " << tempEmail << std::endl;
		
		tempUsers.append(tempUser);
		tempUsers += ',';
		tempLevels.append(tempLevel);
		tempLevels += ',';
	}
	std::cout << "------------------------------------------------" << std::endl;
}

void Admin::set_Deadline(sf::TcpSocket& socket)
{
	std::string input = "x";
	int phase;
	std::string deadLine;

	while(input != "B")
	{
		PacketType Ptype = GET_DEADLINE;
			//Create Packet
		sf::Packet deadLinePacket;
	
			//Send data to packet
		deadLinePacket << id << Ptype;
	
			//Send packet to server
		socket.send(deadLinePacket);	
	
		sf::Packet serverReply;
	
		if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
		{
				//push data from server reply into variables
			serverReply >> Ptype;
			if(Ptype == 6)
			{
				serverReply >> phase >> deadLine;
			}
			else
			{
				std::cout << "EXPECTED: GET_DEADLINE RECIEVED: " << Ptype << std::endl;
			}
		}
		std::string currentPhase;
		switch(phase)
		{
			case 0:
				currentPhase = "Submission";
				break;
			case 1:
				currentPhase = "Bidding";
				break;
			case 2:
				currentPhase = "Reviewing";
				break;
			case 3:
				currentPhase = "Comments";
				break;
			case 4:
				currentPhase = "Rebuttal";
				break;
			case 5:
				currentPhase = "Accepting";
				break;
			case 6:
				currentPhase = "Conference";
				break;
		}
		
		Date deadLineDate(deadLine);
	
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|             Phase Menu            |" << std::endl;
		std::cout << "| Current phase: " << std::setw(19) << std::setfill(' ') << std::left << currentPhase  << "|" << std::endl;
		std::cout << "| Current Deadline: " << deadLineDate << "      |" << std::endl;
		std::cout << "|                                   |" << std::endl;
		std::cout << "|  'P'ush to next phase             |" << std::endl;
		if(phase != -1)
		{
		std::cout << "|  'S'et deadline date              |" << std::endl;
		}
		std::cout << "|  'B'ack                           |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		std::cout << "\n--> ";
		std::cin >> input;
		
		if(input == "P")
		{
			phase++;
			if(phase == 7)
			{
				phase = 0;
			}
		}	
		else if(input == "S")
		{
			std::cout << "Enter new date(DD/MM/YYYY): ";
			std::cin >> deadLine;
		}
		
		if(input == "P" || input == "S")
		{
			PacketType Ptype = SET_DEADLINE;
				//Create Packet
			sf::Packet deadLinePacket;

				//Send data to packet
			deadLinePacket << id << Ptype << phase << deadLine << input;

				//Send packet to server
			socket.send(deadLinePacket);	

			sf::Packet serverReply;
			if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
			{
					//push data from server reply into variables
				serverReply >> Ptype;
				if(Ptype == 7)
				{
					std::cout << text::styleString("Phase has been updated", text::Colour_Green, text::Effect_Bold, text::Bkg_None) << std::endl;
				}
				else
				{
					std::cout << "EXPECTED: GET_DEADLINE RECIEVED: " << Ptype << std::endl;
				}
			}
		}
	}
}

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
//		-- rebuddel phase			4	
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
