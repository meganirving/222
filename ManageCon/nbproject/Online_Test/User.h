/* 
 * File:   user.h
 * Author: Megan
 *
 * Created on October 23, 2014, 3:18 PM
 */

#ifndef USER_H
#define	USER_H

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"

struct Notification
{
	std::string message;
	bool read;
	int id; // IDs start at 1
};

class User
{
	protected:
		std::string id;
		std::string username;
		std::string password;
		std::string email;
		std::vector<Notification> notifs;
		std::vector<std::string> papernames; // for authors: papers they've written. for reviewers: papers they're reviewing
	public:	
		User(){}
		~User(){}
		 virtual void Display(bool&, std::string&, sf::TcpSocket&){}
		
		int unreadNotifs();
		void displayNotifs(sf::TcpSocket&);
		void addNotif(std::string);
		std::string getNotif(Notification);
		void setAllNotifs(std::string, int);
		void loadNotifs(sf::TcpSocket& socket);
		void saveNotifs(sf::TcpSocket& socket);
		
		// some menus
		void notifMenu();
		void reviewCommentMenu(sf::TcpSocket&, std::string);
		void commentMenu(sf::TcpSocket&);
		
		void setUsername(std::string name) { username = name; }
		
		// for the system:
		// find the user with this id, and call addNotif(msg) on them
		void sendNotif(std::string username, std::string msg);
};

#endif
