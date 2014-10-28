#ifndef SHARED_H
#define SHARED_H

typedef int PacketType;
const PacketType LOGIN=0;
const PacketType SIGN_UP=1;
const PacketType GET_USERS=2;
const PacketType ACCEPT_USER=3;
const PacketType WORK_SUBMISSION=4;
const PacketType GET_NOTIFICATION=5;
const PacketType GET_DEADLINE=6;
const PacketType SET_DEADLINE=7;
const PacketType SET_NOTIFICATIONS=8;
const PacketType SET_COMMENT=9;
const PacketType SET_REVIEW=10;
const PacketType GET_REVIEWS=11;
const PacketType GET_NEWS=12;
const PacketType SET_NEWS=13;
const PacketType GET_PHASE=14;
const PacketType OVERIDE_FILENAMES = 15;
const PacketType CHECK_DEADLINE = 16;


const static void testDeadline(sf::TcpSocket& socket, std::string ID)
{
	PacketType Ptype = 16;
	int phase = 0;
	bool changed = false;
	
	sf::Packet deadline;
	deadline << ID << Ptype;
	socket.send(deadline);	
	
	sf::Packet serverReply;
	
	if(socket.receive(serverReply) == sf::Socket::Done)	//check if reply was sent
	{
			//push data from server reply into variables
		serverReply >> Ptype;
		if(Ptype == 16)
		{
			serverReply >> phase >> changed;
			
			if(changed)
			{
				std::string currentPhase;
				switch(phase)
				{
					case 0:
						currentPhase = "Submission";
						break;
					case 1:
						currentPhase = "Bidding";
						break;
					case 2:
						currentPhase = "Reviewing";
						break;
					case 3:
						currentPhase = "Comments";
						break;
					case 4:
						currentPhase = "Rebuttal";
						break;
					case 5:
						currentPhase = "Accepting";
						break;
					case 6:
						currentPhase = "Conference";
						break;
				}
				std::cout << "DEADLINE DATE REACHED: NOW IN " << currentPhase << " PHASE!" << std::endl;
			}
		}
		else
		{
			std::cout << "EXPECTED: CHECK_DEADLINE RECIEVED: " << Ptype << std::endl;
		}
	}
}
#endif
