#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
#include <cstring>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"
#include "User.h"

class Admin : public User
{
	private:
		void ManageUsers(sf::TcpSocket&);
		void getUsers(sf::TcpSocket&, std::string&, std::string&, int&);
	public:
		Admin(const std::string&, const std::string&, const std::string&, const std::string&);
		~Admin(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
};

#endif
