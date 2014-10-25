#ifndef REVIEWER_H
#define REVIEWER_H
#include <iostream>
#include <cstring>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"
#include "User.h"

class Reviewer : public User
{
	private:
	public:
		Reviewer(const std::string&, const std::string&, const std::string&, const std::string&);
		~Reviewer(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
		
		// menu
		void reviewMenu(sf::TcpSocket&);
};

#endif
