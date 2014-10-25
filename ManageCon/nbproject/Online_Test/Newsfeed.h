#ifndef NEWSFEED_H
#define	NEWSFEED_H

#include <queue>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <string>

// data
typedef std::queue<std::string> Newsfeed;

// edit functions
void displayNews(Newsfeed feed);
Newsfeed stringToNews(std::string feed);
std::string stringFromNews(Newsfeed feed);
void addNews(std::string, sf::Socket&, std::string);
Newsfeed getNews(sf::Socket&, std::string);

#endif