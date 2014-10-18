#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <sstream>
#include "sharedglobals.h"
#include "Classes.h"

	//Just a simple GUI function to make sure the mouse is on screen when checking mouse events
bool in_Window(const sf::Mouse&, const sf::Window&, const sf::Vector2<int>&);

int main()
{
	//Stuff for GUI
	/*
	int screen_size_x = 256;
	int screen_size_y = 128;
	sf::Vector2<int> screen_size(screen_size_x, screen_size_y);
	
	sf::RenderWindow window(sf::VideoMode(screen_size_x, screen_size_y), "Online Test [1024x768]");
	*/
	
	
	//Setting random for connection ID(not best method because possible to be the same)
	srand(time(NULL));
	sf::Clock clock;


		// Setting up a text type for GUI
	sf::Font font;
	if (!font.loadFromFile("Padauk.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}
	sf::Text text;
	text.setFont(font);						// set font
	text.setCharacterSize(24);				// set pixel size
	text.setColor(sf::Color::White);		// set colour
	text.setStyle(sf::Text::Bold);			// set style
	text.setString("Test");					// set debug string
	
		//packet contents
	std::string ID = "00000000";			// unique ID for server reference 
	PacketType Ptype;						// message type
	std::string username;					// username for an account
	std::string password;					// password for an account
	std::string input;						// input command (server needs to know aswell)
	std::string userlevel = "-1";			// works out type of user e.g. Author, Admin

		//Connect to server using TCP type connection
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("localhost", 1339);
	if (status != sf::Socket::Done)
	{
		 std::cout << "Could not connect to server" << std::endl;
	}
	else
	{	
		for(int i = 0; i < 8; i++)	//random 8 unique code (so we know whos talking to server)
		{
			ID[i] = static_cast<char>(rand() % 26 + 65);	//random from A - Z
		}
		
			//Initial news feed, will need to connect to server in future
		std::cout << "!------------ NEWS FEED ------------!" << std::endl;
		std::cout << "|                                   |" << std::endl;
		std::cout << "|               INFO                |" << std::endl;
		std::cout << "|                                   |" << std::endl;
		std::cout << "!-----------------------------------!\n" << std::endl;
		
		bool signedIn = false;	//when signed in will change menu
		
		while(input != "Q" && signedIn == false)
		{
			std::cout << "_____________________________________" << std::endl;
			std::cout << "|            Main Menu              |" << std::endl;
			std::cout << "|   'S'ign up                       |" << std::endl;
			std::cout << "|   'L'ogin                         |" << std::endl;
			std::cout << "|   'Q'uit                          |" << std::endl;
			std::cout << "|___________________________________|" << std::endl;
			std::cout << "\n--> ";
			std::cin >> input;
			
			if(input == "L")
			{
					// enter username and password for logging in
 				std::cout << "Enter username: ";
				std::cin >> username;
				std::cout << "Enter password: ";
				std::cin >> password;
				
				Ptype = LOGIN;
				
				
					//Create Packet
				sf::Packet loginPacket;
				
					//Send data to packet
				loginPacket << ID << Ptype << username << password;
				
					//Send packet to server
				socket.send(loginPacket);
				
			}
			else if(input == "S")
			{
	//When signing up make sure username and password are of reasonable length
				do
				{
					std::cout << "Enter username: ";
					std::cin >> username;					
					if(username.length() < 3 || username.length() > 16)
					{
						std::cout << "Username must be between 3 and 16 characters long" << std::endl;
					}
				}
				while(username.length() < 3 || username.length() > 16);
				
				do
				{
					std::cout << "Enter password: ";
					std::cin >> password;
					if(password.length() < 5 || password.length() > 20)
					{
						std::cout << "Username must be between 5 and 20 characters long" << std::endl;
					}
				}
				while(password.length() < 5 || password.length() > 20);
				
				Ptype = SIGN_UP;
				
					//Create Packet
				sf::Packet loginPacket;
				
					//Send data to packet
				loginPacket << ID << Ptype << username << password;
				
					//Send packet to server
				socket.send(loginPacket);
				
					//Create packet to recieve data from server
				sf::Packet serverReply;
			}
			else if(input == "Q")
			{
				std::cout << "Quitting..." << std::endl;
			}
			else
			{
				std::cout << "Invalid Input" << std::endl;
			}	
				//Create packet to recieve data from server
			sf::Packet serverReply;
			
			if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
			{
					//push data from server reply into variables
				serverReply >> ID >> Ptype;
				switch(Ptype)
				{
					case 0:
						serverReply >> userlevel;
						if(userlevel == "ADMIN" || userlevel == "REVIEWER" || userlevel == "AUTHOR")
						{
							signedIn = true;
						}
						else if(userlevel == "UNAPPROVED")
						{
							std::cout << "Still waiting admin approval" << std::endl;
						}
						else if(userlevel == "BAD-PASS")
						{
							std::cout << "Invalid password" << std::endl;
						}
						else
						{
							std::cout << "Non-existing account" << std::endl;
						}
						break;
					case 1:
						serverReply >> userlevel;
						if(userlevel == "NON-EXISTING")
						{
							std::cout << "Created account now awaiting approval" << std::endl;
						}
						else
						{
							std::cout << "Account name already exists" << std::endl;
						}
						break;
				}
			}
		}
		User* player;
		if(userlevel == "AUTHOR")
		{
			player = new Author(username, password);
		}
		else if(userlevel == "REVIEWER")
		{
			player = new Reviewer(username, password);
		}
			//A user has successfully signed in
		while(signedIn == true)
		{
			player->Display(signedIn, input);
			/*
			if(userlevel == "AUTHOR")
			{
				std::cout << "_____________________________________" << std::endl;
				std::cout << "|            Logged In              |" << std::endl;
				std::cout << "|   'S'ubmit work                   |" << std::endl;
				std::cout << "|   'E'mails                        |" << std::endl;
				std::cout << "|   'Q'uit                          |" << std::endl;
				std::cout << "|___________________________________|" << std::endl;
				std::cout << "\n--> ";
				std::cin >> input;
				if(input == "Q")
				{
					signedIn = false;
				}
			}
			else if(userlevel == "REVIEWER")
			{
				std::cout << "_____________________________________" << std::endl;
				std::cout << "|            Logged In              |" << std::endl;
				std::cout << "|   'R'eview work                   |" << std::endl;
				std::cout << "|   'S'et prefences				  |" << std::endl;
				std::cout << "|   'E'mails                        |" << std::endl;
				std::cout << "|   'Q'uit                          |" << std::endl;
				std::cout << "|___________________________________|" << std::endl;
				std::cout << "\n--> ";
				std::cin >> input;
				if(input == "Q")
				{
					signedIn = false;
				}
			}
			else
			{
				std::cout << "_____________________________________" << std::endl;
				std::cout << "|            Logged In              |" << std::endl;
				std::cout << "|   'M'anage Users                  |" << std::endl;
				std::cout << "|   'S'et deadlines            	  |" << std::endl;
				std::cout << "|   'E'mails                        |" << std::endl;
				std::cout << "|   'Q'uit                          |" << std::endl;
				std::cout << "|___________________________________|" << std::endl;
				std::cout << "\n--> ";
				std::cin >> input;
				if(input == "M")
				{
					
				}
				else if(input == "Q")
				{
					signedIn = false;
				}
			}*/
		}
	}
    

		//if we decide to use GUI then we will need to implement inside this while loop

    /*while (window.isOpen())
    {
    	//std::cout << "in game" << std::endl;
		double deltaTime = clock.restart().asSeconds();
		sf::Event event;
		sf::Mouse mouse;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		int action = input.GetAction(deltaTime);
	
	
		if (mouse.isButtonPressed(mouse.Left))
		{
			if(in_Window(mouse, window, screen_size))
			{
				socket.send(ID, sizeof(ID));
				socket.send(username, sizeof(username));
				socket.send(password, sizeof(password));
				
				char* userlevel;
				std::size_t recieved = 0;
			}
		}
	
        window.clear();
        window.draw(text);
        window.display();
    }*/

    return 0;
}

bool in_Window(const sf::Mouse& mouse, const sf::Window& window, const sf::Vector2<int>& screen_size)
{
	return(mouse.getPosition(window).x < screen_size.x && mouse.getPosition(window).x > 0 && mouse.getPosition(window).y < screen_size.y && mouse.getPosition(window).y > 0);
}
