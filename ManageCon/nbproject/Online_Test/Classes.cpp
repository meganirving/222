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
	std::cout << "|   'R'eply to reviews              |" << std::endl;
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "Q")
	{
		signedIn = false;
	}
}
void Author::SubmitWork(){

	std::string ID = "01010101";
	PacketType PType = WORK_SUBMISSION; 
	sf::Packet packet;
	sf::TcpSocket socket;
	std::string filename;
	std::string keywords[5];

	sf::Socket::Status status = socket.connect("localhost", 1338);
	if (status != sf::Socket::Done)
	{
		 std::cout << "Could not connect to server" << std::endl;
	}

	std::cout << "Please enter filename to submit" << std::endl;
	std::cin >> filename;

	std::cout << "Please enter keywords for paper" << std::endl;

	for(int i = 0; i < 5; i++){
		std::cin >> keywords[i];
	}
	
	std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	// Get file size to send to server
	File.seekg(0, File.end);
	int size = File.tellg();

	std::cout << "File size is: " << size << std::endl;
	//File back with filename, keywords and size of file to expect
	packet << ID << PType << filename << username << keywords[0] << 
			   keywords[1] << keywords[2] <<keywords[3] << keywords[4] << size;


	
	
	if(File.good())
		{

		std::cout << "File is good" << std::endl;

		socket.send(packet);
		//Make sure file pointer is at beginning
		File.seekg(0, std::ios::beg);

		char* Buffer = new char[4096];

		while(File.read(Buffer, sizeof(Buffer)))
		{

			socket.send(Buffer, sizeof(Buffer));
		}
		
		File.close();
		delete[] Buffer;

	
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
	std::cout << "|   'B'id on papers                 |" << std::endl;
	std::cout << "|   'R'eply to reviews              |" << std::endl;
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
	std::cout << "|	  'A'ccepted Papers               |" << std::endl;
	std::cout << "|   'S'et deadlines            	  |" << std::endl;
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
	UserMenu(socket);
}

void Admin::UserMenu(sf::TcpSocket& socket)
{
	std::string users;
	std::string levels;
	int userCount;
	std::string input = "x";
	char acceptUser = 'x';
	int usernumber = -1;
	
	
	while(input != "Q")
	{
		getUsers(socket, users, levels, userCount);
		std::string myusers = users;
		std::string mylevels = levels;
	
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|            User Menu              |" << std::endl;
		std::cout << "|   '#' Accept user number          |" << std::endl;
		std::cout << "|   'Q'uit                          |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		std::cout << "\n--> ";
		std::cin >> input;
		if(input != "Q")
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
				std::cout << usernumber << ": " << designatedUser << " wishes to become a " << designatedLevel << std::endl;
				
				
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
		else
		{
			std::cout << "Quiting" << std::endl;
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
