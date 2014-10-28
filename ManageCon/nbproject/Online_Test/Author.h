#ifndef AUTHOR_H
#define AUTHOR_H
#include <iostream>
#include <cstring>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"
#include "User.h"

class Author : public User
{
	private:
	public:
		Author(){}
		Author(const std::string&, const std::string&, const std::string&, const std::string&);
		~Author(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
		void SubmitWork(sf::TcpSocket&);
};

#endif
