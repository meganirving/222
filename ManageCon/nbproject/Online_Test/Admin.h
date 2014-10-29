#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"
#include "User.h"
#include "Date.h"
#include "Review.h"

class Admin : public User
{
	private:
		void ManageUsers(sf::TcpSocket&);
		void getUsers(sf::TcpSocket&, std::string&, std::string&, int&);
		
		
	public:
		Admin(const std::string&, const std::string&, const std::string&, const std::string&);
		~Admin(){}
		void paperMenu(sf::TcpSocket&); 
		void Display(bool&, std::string&, sf::TcpSocket&);
		void set_Deadline(sf::TcpSocket&);
		void ManagePapers(sf::TcpSocket&);
		void AcceptPapers(std::string, sf::TcpSocket&, std::string);
		void RejectPapers(std::string, sf::TcpSocket&, std::string);
		void ManageNews(sf::TcpSocket&);
		void AddNews(sf::TcpSocket&);
		void ClearNews(sf::TcpSocket&);
};

#endif
