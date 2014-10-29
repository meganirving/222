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
	
	//MAybe create a function that sends over 1 file, call it twice
	PacketType PType = WORK_SUBMISSION; 
	sf::Packet packet;
	int input_num = -1;
	//char Buffer[4096] = {};
	std::string filename;
	std::string filename2;
	std::string authors[3] = {""};
	std::string keywords[5] = {""};

	std::cout << "Please enter filename of full document" << std::endl;
	std::cin >> filename;

	std::cout << "Please enter Author's of paper (max 3, 'end' to stop)" << std::endl;

	do
	{
		input_num++;
		std::getline(std::cin, authors[input_num]); 
	
		
	}while(input_num < 3 && authors[input_num] != "end");
	
	input_num = -1;

	std::cout << "Please enter paper for review filename" << std::endl;
	std::cin >> filename2;
	
	std::cout << "Please enter keywords for paper (5 max, 'end' to stop)" << std::endl;

	do
	{
		input_num++;
		std::cin >> keywords[input_num];
		
	}while(input_num < 4 && keywords[input_num] != "end");
	
	std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	std::ifstream File2(filename2.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	
	//Get file sizes
	int size2 = File2.tellg();
	int size = File.tellg();
	//File back with filename, keywords and size of file to expect
	packet << id << PType << filename << filename2 << username << keywords[0] << 
	       keywords[1] << keywords[2] <<keywords[3] << keywords[4] << size << size2;

	for(int i = 0; i < 3; i++){

		packet << authors[i];
	}
	
	if(File.good() && File2.good())
	{

		socket.send(packet);
		//Make sure file pointer is at beginning
		File.seekg(0, std::ios::beg);
		File2.seekg(0, std::ios::beg);

		char* Buffer = new char[size];
		int full_size = 0;
		//Send file to server
		while(File.read(Buffer, sizeof(Buffer)))
		{
			
			socket.send(Buffer, sizeof(Buffer));

		}
		
		memset(Buffer, 0, sizeof(Buffer));
		File.close();

		while(File2.read(Buffer, sizeof(Buffer)))
		{
			socket.send(Buffer, sizeof(Buffer));
			memset(Buffer, 0, sizeof(Buffer));
			
		}

		File2.close();
		delete[] Buffer;
		system("clear");
		std::cout << "\033[1;36mPaper Submission successful\033[0m" << std::endl;
		
		
	}else
		{
			std::cout << "Bad file name" << std::endl;
		}

}

