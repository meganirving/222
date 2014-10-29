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

	// header
	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	
	// conditional options
	if (currPhase == 1) // bidding phase
	{
		std::cout << "|   'B'id on papers                 |" << std::endl;
	}
	if (currPhase == 2) // review phase
	{
		std::cout << "|   'R'eview works                  |" << std::endl;
	}
	if (currPhase == 3)
	{
		std::cout << "|   'C'omment on reviews            |" << std::endl;
	}
	
	// footer
	notifMenu();
	std::cout << "|   'S'et prefences                 |" << std::endl;
	std::cout << "|   'Q'uit                          |" << std::endl;
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	
	// handle input
	std::cin >> input;
	if(input == "Q")
	{
		signedIn = false;
	}
	else if(input == "N")
	{
		displayNotifs(socket);
	}
	else if (input == "B" && currPhase == 1)
	{
		biddingMenu(socket);
	}
	else if (input == "R" && currPhase == 2)
	{
		reviewMenu(socket);
	}
	else if (input == "C" && currPhase == 3)
	{
		commentMenu(socket);
	}
}

void Reviewer::biddingMenu(sf::TcpSocket& socket)
{
	std::string papernames;	
	std::string tempnames;
	int count;
	std::string input;
	int num;
	int bid_input;
	
	while(input != "B")
	{
		PacketType Ptype = GET_PAPERS_LIST;
		sf::Packet biddingPacket;
		biddingPacket << id << Ptype;
		socket.send(biddingPacket);
		sf::Packet serverReply;
			
		if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
		{
				//push data from server reply into variables
			serverReply >> Ptype;
			if(Ptype == 17)
			{
				serverReply >> papernames >> count;
			}
			else
			{
				std::cout << "EXPECTED: GET_PAPERS_LIST RECIEVED: " << Ptype << std::endl;
			}
		}
	
		std::cout << "There are currently " << count << " papers to be bid on: " << std::endl;
	
		
		int pos = 0;
		std::string tempName;
		std::cout << "List of papers: " << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
		tempnames = papernames;
		for(int i = 0; i < count; i++)
		{
				//get user name
			pos = tempnames.find_first_of(",");
			tempName = (tempnames.substr(0, pos));
			tempnames = tempnames.substr(pos+1, tempnames.length()-pos);
			std::cout << "|- " << i+1 << ": " << tempName << std::endl;
		}
		std::cout << "------------------------------------------------" << std::endl;
	
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|          Bidding Menu             |" << std::endl;
		std::cout << "|   '#' Select paper                |" << std::endl;
		std::cout << "|   'B'ack                          |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		std::cout << "\n--> ";
		std::cin >> input;
		if(input != "B")
		{
			num = atoi(input.c_str());
			if(num > count || num < 1)
			{
				
				std::cout << "Paper " << num << " doesn't exist." << std::endl;
			}
			for(int i = 0; i < count; i++)
			{
				if(num == i+1)
				{
						//get user name
					tempnames = papernames;
					pos = 0;
					for(int j = 0; j < num; j++)
					{
						pos = tempnames.find_first_of(",");
						tempName = (tempnames.substr(0, pos));
						tempnames = tempnames.substr(pos+1, tempnames.length()-pos);
					}
					do
					{
						std::cout << std::endl;
						std::cout << "\tBid prefence for " << tempName << ": " << std::endl;
						std::cout << "\t 1. Conflict of interest" << std::endl;
						std::cout << "\t 2. Prefer not to get" << std::endl;
						std::cout << "\t 3. Unphased" << std::endl;
						std::cout << "\t 4. Prefered" << std::endl;
						std::cout << "\t 5. Highly prefered" << std::endl; 
						std::cout << "\n--> ";
						std::cin >> bid_input;
					}
					while(bid_input > 5 || bid_input < 1);
					PacketType Ptype = ADD_TO_BIDS;
					biddingPacket.clear();
					biddingPacket << id << Ptype << tempName << username << bid_input;
					socket.send(biddingPacket);
					serverReply.clear();
					
					if(socket.receive(serverReply) == sf::Socket::Done)
					{
						serverReply >> Ptype;
						if(Ptype == 18)
						{
							std::cout << "Bid placed" << std::endl;
						}
						else
						{
							std::cout << "EXPECTED: ADD_TO_BIDS RECIEVED: " << Ptype << std::endl;
						}
					}
					i = count;
				}
			}
		}
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
				generateForm(username, *itr);
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

