#include <fstream>
#include <iostream>
#include "newsfeed.h"

// newsfeed -> string -> newsfeed
Newsfeed stringToNews(std::string feed)
{
	Newsfeed result;
	
	// loop through the string and add each news item to the queue
	int pos = feed.find_first_of("*");
	while(pos != std::string::npos)
	{
		std::string test = feed.substr(0, pos);
		result.push( test );
		feed = feed.substr(pos+1, feed.length()-pos);
		pos = feed.find_first_of("*");
	}

	return result;
}
std::string stringFromNews(Newsfeed feed)
{
	std::string result;
	
	// loop through feed and add each news item to the string
	while (!feed.empty())
	{
		result.append(feed.front());
		feed.pop();
		result.append("*");
	}
	
	return result;
}


void displayNews(Newsfeed feed)
{
	std::cout << "|\tNewsfeed:" << std::endl;
	std::string news;
	while (!feed.empty())
	{
		news = feed.front();
		feed.pop();
		std::cout << "|\t" << news << std::endl;
	}
}

Newsfeed getNews(sf::Socket& socket, std::string ID)
{
	// send off a request to the server
	sf::Packet newsPacket;
	PacketType pType = GET_NEWS;
	newsPacket << ID << pType;
	socket.send(newsPacket);
	
	// handle the response
	sf::Packet serverReply;
	std::string news;
	if(socket.receive(serverReply) == sf::Socket::Done)
	{
		serverReply >> news;
	}
	
	// turn the response into news and return
	return stringToNews(news);
}

// update newsfeed with the news string, save to file
void addNews(std::string news, sf::Socket& socket, std::string ID)
{
	// get all the news
	Newsfeed newsfeed = getNews(socket, ID);
	
	// if the news is full, get rid of the oldest news
	int size = newsfeed.size();
	if (newsfeed.size() == 3)
	{
		newsfeed.pop();
	}
	
	// add the new news
	news = news + "*\n";
	newsfeed.push(news);
	
	// turn into a string
	std::string feed = stringFromNews(newsfeed);
	
	// send off to the server
	PacketType pType = SET_NEWS;
	sf::Packet newsPacket;
	newsPacket << ID << pType << feed;
	socket.send(newsPacket);
}