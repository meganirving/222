#ifndef USER_H
#define USER_H
#include <iostream>
class User
{
	protected:
		std::string username;
		std::string password;
		std::string email;
	public:	
		User(){}
		~User(){}
		virtual void Display(bool&, std::string&);
};

class Author : public User
{
	private:
	public:
		Author(const std::string&, const std::string&, const std::string&);
		~Author(){}
		void Display(bool&, std::string&);
};

class Reviewer : public User
{
	private:
	public:
		Reviewer(const std::string&, const std::string&, const std::string&);
		~Reviewer(){}
		void Display(bool&, std::string&);
};

class Admin : public User
{
	private:
	public:
		Admin(const std::string&, const std::string&, const std::string&);
		~Admin(){}
		void Display(bool&, std::string&);
};

#endif
