#include <iostream>
#include <fstream>
#include <sstream>  
#include <string>
#include <cstdlib>
#include "User.h"

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
void User::displayNotifs()
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
	
	// return to previous menu somehow
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

void User::saveNotifs()
{
	// get the filename
	std::string fname = username;
	fname.append("_notifs.txt");
	
	// open the file
	std::ofstream ofile;
	ofile.open(fname.c_str(), std::ofstream::out);
	
	// save the notifications to the file
	std::vector<Notification>::iterator itr = notifs.begin();
	while (itr != notifs.end())
	{
		ofile << getNotif(*itr) << std::endl;
		itr++;
	}
	
	// close file
	ofile.close();
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
			
			std::cout << "GOT: " << count << " " << notifs << std::endl;
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
	std::cout << count << std::endl;
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
