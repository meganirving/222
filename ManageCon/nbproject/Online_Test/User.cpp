#include <iostream>
#include <fstream>
#include <sstream>  
#include <string>
#include <cstdlib>
#include "User.h"
#include "Review.h"

// review menus
// MENUS
// for authors and reviewers: go through papers and view reviews
void User::commentMenu(sf::TcpSocket& socket) 
{
	std::vector<std::string> papers;
	std::vector<std::string>::iterator itr = papers.begin();
	
	// display menu until go back
	char input = 'x';
	while (input != 'B')
	{
		// display paper name and options
		std::cout << "\n|\t\t" << *itr << "\t\t|";
		std::cout << "|\t'V'iew Reviews\t\t|" << std::endl;
		std::cout << "|\t'P'rev Paper\t\t|" << std::endl;
		std::cout << "|\t'N'ext Paper\t\t|" << std::endl;
		std::cout << "|\t'B'ack\t\t|" << std::endl;
		
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
				if (temp != papers.end())
				{
					itr = temp;
				}
				break;
			}
			case 'P': // view prev paper
			{
				if (itr != papers.begin())
				{
					itr--;
				}
				break;
			}
		}
	}
}
// for commenting on reviews
void User::reviewCommentMenu(sf::TcpSocket& socket, std::string papername) 
{
	// reviewer variable
	std::vector<Review> reviews;
	
	// get all the reviews for that paper
	reviews = getAllReviews(socket, id, papername);
	
	std::vector<Review>::iterator itr = reviews.begin();
	
	// display menu until exit
	char input = 'x';
	while (input != 'B')
	{
		// display review and comments
		std::cout << "\n" << itr->reviewName << "'s Review:" << std::endl;
		itr->display();

		// display options
		std::cout << "\n|\t'C'omment on review\t|" << std::endl;
		if (reviews.size() > 1)
		{
			std::cout << "|\t'N'ext review\t\t|" << std::endl;
			std::cout << "|\t'P'rev review\t\t|" << std::endl;
		}
		std::cout << "|\t'B'ack\t\t\t|\n" << std::endl;

		// get input
		std::cin >> input;

		// deal with input
		switch(input)
		{
			case 'C': // gets a comment from the user and adds it to the review
			{
				// get comment
				std::string comment;
				std::cout << "Write short comment here, using the enter key to finish:" << std::endl;
				getline(std::cin, comment);
				getline(std::cin, comment);

				std::string final = "\n\n" + username + ": " + comment;

				// add to list
				itr->comments.append(final);
				
				// save comment
				addComment(socket, id, username, itr->paperName, reviews);
				
				// give feedback and return
				std::cout << "\nComment successfully added" << std::endl;
				input = 'B';
				break;
			}
			case 'N': // view next review
			{
				std::vector<Review>::iterator temp = itr;
				temp++;
				if (temp != reviews.end())
				{
					itr = temp;
				}
				break;
			}
			case 'P': // view prev review
			{
				if (itr != reviews.begin())
				{
					itr--;
				}
				break;
			}
		}
	} // input == 'B', exit
}


// adds a new notification to the user
void User::addNotif(std::string msg)
{
	// create a new notification
	Notification temp;
	
	// set the data
	temp.read = false;
	temp.message = msg;
	temp.id = notifs.size() + 1;
	
	// add to vector
	notifs.push_back(temp);
}

// gets the amount of unread notifications a user has
int User::unreadNotifs()
{
	// reset unread count
	int unread = 0;
	
	// loop through all notificatons
	std::vector<Notification>::iterator itr = notifs.begin();
	while (itr != notifs.end())
	{
		// if it's unread, increment the counter
		if (itr->read == false)
		{
			unread++;
		}
		itr++;
	}
	
	// return the count
	return unread;
}

// call this function inside the author and reviewer menu functions
void User::notifMenu()
{
	// get the amount of unread notifications
	int unread = unreadNotifs();
	std::cout << "|   'N'otifications (" << unread << " unread)      |" << std::endl;
}

// called when the user has selected 'n' in their main menu
void User::displayNotifs(sf::TcpSocket& socket)
{
	// get the first unread notification, or final notification in the list
	std::vector<Notification>::iterator itr = notifs.begin();
	while (itr != notifs.end() && itr->read == true)
	{
		itr++;
	}
	// if it went too far, go back
	if (itr == notifs.end())
	{
		itr--;
	}
	
	char input = 'x';
	while (input != 'B')
	{
		// display current notif and set it to read
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|              Message (" << itr->id << "/" << notifs.size() << ")        |" << std::endl;
		std::cout << itr->message << "\n\n";
		itr->read = true;
		
		// display menu
		std::cout << "|                                   |" << std::endl;
		std::cout << "|   'N'ext Notification             |" << std::endl;
		std::cout << "|   'P'rev Notification             |" << std::endl;
		std::cout << "|   'B'ack                          |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		
		// get input
		std::cin >> input;
		
		// previous notification
		if (input == 'N')
		{
			// if this isn't the last notif, increment the iterator
			std::vector<Notification>::iterator temp = itr;
			temp++;
			if (temp != notifs.end())
			{
				itr = temp;
			}
		}
		// previous notif
		else if (input == 'P')
		{
			// if this isn't the first notif, decrement the iterator
			if (itr != notifs.begin())
			{
				itr--;
			}
		}
	}
	
	// save notifs
	saveNotifs(socket);
}

// turns a notif into a string
std::string User::getNotif(Notification notif)
{
	
	std::stringstream temp;
	temp << notif.message;
	temp << ",";
	temp << notif.id;
	temp << ",";
	temp << notif.read;
	std::string result = temp.str();
	return result;
}

void User::saveNotifs(sf::TcpSocket& socket)
{
	// get the filename
	std::string fname = username;
	fname.append("_notifs.txt");
	std::string toReturn = "";
	
	// get all notifs into a string
	for (int i = 0; i < notifs.size(); i++)
	{
		toReturn.append(notifs[i].message);
		toReturn.append("\n");
	}
	
	// send to server
	sf::Packet packet;
	PacketType pType = SET_NOTIFICATIONS;
	packet << id << pType << fname << toReturn;
	socket.send(packet); 
}

void User::loadNotifs(sf::TcpSocket& socket)
{
	// send username to server
	PacketType pType = GET_NOTIFICATION;
	sf::Packet packet;
	packet << id << pType << username;
	socket.send(packet);
	
	// get info back from server
	std::string notifs;
	int count;
	sf::Packet serverReply;
	if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
	{
			//push data from server reply into variables
		serverReply >> pType;
		
		if(pType == GET_NOTIFICATION)
		{
			serverReply >> count >> notifs;
		}
		else
		{
			std::cout << "EXPECTED GET_NOTIFICATION GOT: " << pType << std::endl;
		}
	}
	// get notifs
	setAllNotifs(notifs, count);

}

void User::setAllNotifs(std::string nots, int count)
{
	int amt = 0;
	while (amt < count)
	{
		// get the next notif in string format
		int pos = 0;
		pos = nots.find_first_of("~");
		std::string notif = nots.substr(0, pos);
		nots = nots.substr(pos+1, nots.length()-pos);
		
		// turn that string into a notif
		Notification temp;
		pos = notif.find_first_of(",");
		temp.message = notif.substr(0, pos);
		notif = notif.substr(pos+1, notif.length()-pos);
		pos = notif.find_first_of(",");
		temp.id = atoi((notif.substr(0, pos)).c_str());
		notif = notif.substr(pos+1, notif.length()-pos);
		temp.read = atoi(notif.c_str());
		
		// add to the list and increment the count
		notifs.push_back(temp);
		amt++;
	}
}
