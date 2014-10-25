#include "Author.h"

Author::Author(const std::string& ID, const std::string& name, const std::string& pass, const std::string& myemail)
{
	id = ID;
	username = name;
	password = pass;
	email = myemail;
}

void Author::Display(bool& signedIn, std::string& input, sf::TcpSocket& socket)
{
	loadNotifs(socket);

	std::cout << "_____________________________________" << std::endl;
	std::cout << "|            Logged In              |" << std::endl;
	std::cout << "|   'S'ubmit work                   |" << std::endl; // only available in phase 0
	std::cout << "|   'C'omment on reviews            |" << std::endl; // only available in phase 4
	std::cout << "|   'Q'uit                          |" << std::endl;
	notifMenu();
	std::cout << "|___________________________________|" << std::endl;
	std::cout << "\n--> ";
	std::cin >> input;
	if(input == "S")
	{
		SubmitWork();
	}
	else if(input == "N")
	{
		displayNotifs(socket);
	}
	else if (input == "C")
	{
		commentMenu(socket);
	}
	else if(input == "Q")
	{
		signedIn = false;
	}
}
void Author::SubmitWork()
{
	std::string ID = "01010101";
	PacketType PType = WORK_SUBMISSION; 
	sf::Packet packet;
	sf::TcpSocket socket;
	std::string filename;
	std::string keywords[5];

	sf::Socket::Status status = socket.connect("localhost", 1338);
	if (status != sf::Socket::Done)
	{
		 std::cout << "Could not connect to server" << std::endl;
	}

	std::cout << "Please enter filename to submit" << std::endl;
	std::cin >> filename;

	std::cout << "Please enter keywords for paper" << std::endl;

	for(int i = 0; i < 5; i++){
		std::cin >> keywords[i];
	}
	
	std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	// Get file size to send to server
	File.seekg(0, File.end);
	int size = File.tellg();

	std::cout << "File size is: " << size << std::endl;
	//File back with filename, keywords and size of file to expect
	packet << id << PType << filename << keywords[0] << 
			   keywords[1] << keywords[2] <<keywords[3] << keywords[4] << size;
	
	if(File.good())
	{
		std::cout << "File is good" << std::endl;

		socket.send(packet);
		//Make sure file pointer is at beginning
		File.seekg(0, std::ios::beg);

		char* Buffer = new char[4096];

		while(File.read(Buffer, sizeof(Buffer)))
		{
			socket.send(Buffer, sizeof(Buffer));
		}	
		File.close();
		delete[] Buffer;
	}
}

