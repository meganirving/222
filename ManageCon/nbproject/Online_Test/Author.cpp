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
	while(input != "Q")
	{
		loadNotifs(socket);
		getPhase(socket);
		// header
		std::cout << "_____________________________________" << std::endl;
		std::cout << "|            Logged In              |" << std::endl;
	
		// conditional options
		if (currPhase == 0) // submission phase
		{
			std::cout << "|   'S'ubmit work                   |" << std::endl;
		}
		if (currPhase == 4) // rebuttal phase
		{
			std::cout << "|   'C'omment on reviews            |" << std::endl;
		}
	
		// footer
		notifMenu();
		std::cout << "|   'Q'uit                          |" << std::endl;
		std::cout << "|___________________________________|" << std::endl;
		std::cout << "\n--> ";
	
		// handle input
		std::cin >> input;
		if(input == "S" && currPhase == 0)
		{
			SubmitWork(socket);
		}
		else if(input == "N")
		{
			displayNotifs(socket);
		}
		else if (input == "C" && currPhase == 4)
		{
			commentMenu(socket);
		}
		else if(input == "Q")
		{
			signedIn = false;
		}
	}
}

// submits a work
void Author::SubmitWork(sf::TcpSocket& socket)
{
	PacketType PType = WORK_SUBMISSION; 
	sf::Packet packet;
	std::string filename;
	std::string filename2;
	std::string keywords[5];

	std::cout << "Please enter filename of full document" << std::endl;
	std::cin >> filename;
	
	std::cout << "Please enter anonymouse paper filename" << std::endl;
	std::cin >> filename2;
	
	

	std::cout << "Please enter keywords for paper" << std::endl;

	for(int i = 0; i < 5; i++){
		std::cin >> keywords[i];
	}
	
	std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	std::ifstream File2(filename2.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	
	// Get file size to send to server
	File.seekg(0, File.end);
	File2.seekg(0, File2.end);
	int size2 = File2.tellg();
	int size = File.tellg();

	std::cout << "File size is: " << size << std::endl;
	std::cout << "File2 size is: " << size2 << std::endl;
	//File back with filename, keywords and size of file to expect
	packet << id << PType << filename << filename2 << username << keywords[0] << 
			   keywords[1] << keywords[2] <<keywords[3] << keywords[4] << size << size2;
	if(File.good())
	{
		std::cout << "File is good" << std::endl;

		socket.send(packet);
		//Make sure file pointer is at beginning
		File.seekg(0, std::ios::beg);
		File2.seekg(0, std::ios::beg);

		char* Buffer = new char[4096];
		char* Buffer2 = new char[4096];

		while(File.read(Buffer, sizeof(Buffer)))
		{
			socket.send(Buffer, sizeof(Buffer));
		}
		File.close();
		/*while(File2.read(Buffer2, sizeof(Buffer2)))
		{
			socket.send(Buffer2, sizeof(Buffer2));
		}*/
		
		File2.close();
		delete[] Buffer;
		delete[] Buffer2;
	}
}

