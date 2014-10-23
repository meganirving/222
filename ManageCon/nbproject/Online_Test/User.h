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
	public:	
		User(){}
		~User(){}
		//TO UNCOMMENT: virtual void Display(bool&, std::string&, sf::TcpSocket&);
		int unreadNotifs();
		void notifMenu();
		void displayNotifs();
		void addNotif(std::string);
		std::string getNotif(Notification);
		void setAllNotifs(std::string, int);
		bool loadNotifs();
		void saveNotifs();
		
		void setUsername(std::string name) { username = name; }
		
		// for the system:
		// find the user with this id, and call addNotif(msg) on them
		void sendNotif(std::string username, std::string msg);
};

#endif	/* USER_H */

