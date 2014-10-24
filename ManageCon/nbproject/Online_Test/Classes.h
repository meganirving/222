#ifndef USER_H
#define USER_H
#include <iostream>
#include <cstring>
#include <fstream>
#include <SFML/Network.hpp>
#include "sharedglobals.h"
class User
{
	protected:
		std::string id;
		std::string username;
		std::string password;
		std::string email;
	public:	
		User(){}
		~User(){}
		virtual void Display(bool&, std::string&, sf::TcpSocket&);
};

class Author : public User
{
	private:
	public:
		Author(const std::string&, const std::string&, const std::string&, const std::string&);
		~Author(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
		void SubmitWork();
};

class Reviewer : public User
{
	private:
	public:
		Reviewer(const std::string&, const std::string&, const std::string&, const std::string&);
		~Reviewer(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
};

class Admin : public User
{
	private:
		void ManageUsers(sf::TcpSocket&);
		void UserMenu(sf::TcpSocket&);
		void getUsers(sf::TcpSocket&, std::string&, std::string&, int&);
	public:
		Admin(const std::string&, const std::string&, const std::string&, const std::string&);
		~Admin(){}
		void Display(bool&, std::string&, sf::TcpSocket&);
};

#endif
