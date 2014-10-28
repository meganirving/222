/* 
 * File:   reviewer.cpp
 * Author: Megan
 * 
 * Created on October 23, 2014, 6:41 PM
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector> 
#include <stdlib.h>
#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include "Review.h"
#include "sharedglobals.h"


// displays a review
void Review::display()
{
	std::cout << "Average Score: " << avScore << std::endl;
	std::cout << "Scores: \nOverall Evaluation: " << overallEval << "\nReviewer's Confidence: " << reviewConfidence << "\nRelevance: " << relevance << "\nOriginality: " << originality << "\nSignificance: " << significance << "\nPresentation: " << presentation << "\nTechnical Quality: " << techQuality << "\nEvaluation: " << eval << std::endl;
	std::cout << "\nDetailed Comments: " << reviews << std::endl;
	std::cout << "\nComments: " << comments << std::endl;
 }
// generates the average score of the review and sets it
void Review::generateAvScore()
{
	// get the average of the main evalations
	float total = eval + originality + presentation + relevance + reviewConfidence + techQuality + significance;
	total /= 7;
	
	// add the "overall evaluation"
	total += overallEval;
	
	// set the score
	avScore = total;
}
// turns it into a string for easy C/S
std::string Review::toString()
{
	// create stream
	std::stringstream stream;
	
	// add the review to the stream
	stream << "~"; // start of review
	stream << reviewName << "*" << paperName << "*\n";
	stream << avScore << "," << overallEval << "," << reviewConfidence << "," << relevance << "," << originality << "," << significance << "," << presentation << "," << techQuality << "," << eval;
	stream << "*" << reviews << "*";
	stream << comments;
	stream << "~\n"; // end of review
	
	// return the stream as a string	  
	return stream.str();
}
// turns a string into a review
void Review::fromString(std::string stream)
{	
	// meta data
	int pos = stream.find_first_of("*");
	reviewName = stream.substr(1, pos-1);
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of("*");
	paperName = stream.substr(0, pos);
	stream = stream.substr(pos+1, stream.length()-pos);
	
	// scores
	pos = stream.find_first_of(",");
	avScore = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	overallEval = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	reviewConfidence = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	relevance = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	originality = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	significance = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	presentation = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of(",");
	techQuality = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	pos = stream.find_first_of("*");
	eval = atof((stream.substr(0, pos)).c_str());
	stream = stream.substr(pos+1, stream.length()-pos);
	
	// detailed comments
	pos = stream.find_first_of("*");
	reviews = stream.substr(0, pos);	
	stream = stream.substr(pos+1, stream.length()-(pos+1));
	
	// get comments
	pos = stream.find_first_of("~");
	std::string coms = stream.substr(0, pos);
	comments = stream.substr(0, pos-2);
}


// MISC local functions:
// creates a form and saves it
void generateForm(std::string username, std::string papername)
{
	// open the file
	std::string fname = "Review.txt";
	std::ofstream ofile;
	ofile.open(fname.c_str(), std::ofstream::out);
	
	// create form
	std::string author = "anonymous";
	std::string reviewername = username;
	std::string linebreak = "-------------------------------------------------";
	
	ofile << linebreak << std::endl;
	ofile << "--- \tConference Management System" << std::endl;
	ofile << "--- This form is automatically processed by the system. Please fill it in and submit it." << std::endl;
	ofile << "--- When filling out this review form, please keep these rules in mind:" << std::endl;
	ofile << "--- (1) Lines beginning with '---' are comments, and will not be processed by the system." << std::endl;
	ofile << "--- (2) Lines beginning with '***' are important to the system. Do not remove or modify these lines, or your review will be rejected." << std::endl;
	ofile << linebreak << std::endl;
	ofile << "*** Paper Title: " << papername << std::endl;
	ofile << "*** Paper's Author: " << author << std::endl;
	ofile << "*** Reviewed By: " << reviewername << std::endl;
	ofile << linebreak << std::endl;
	ofile << "*** Review: " << std::endl;
	ofile << "--- Please leave a detailed review. This review will be sent to the author." << std::endl;
	ofile << ". Strengths of the Paper: \n\n" << std::endl;
	ofile << ". Weaknesses of the Paper: \n\n" << std::endl;
	ofile << ". Detailed comments and suggestions for improving the paper: \n\n" << std::endl;
	ofile << ". Would this paper be more suitable as a short paper? \n\n" << std::endl;
	ofile << ". Should this paper be considered for a best paper award? \n\n" << std::endl;
	ofile << linebreak << std::endl;
	ofile << "*** Evaluation: " << std::endl;
	ofile << "--- In the evaluations below, remove the '---' on the line with your evaluation. You can also remove the irrelevant lines\n" << std::endl;
	ofile << "*** OVERALL EVALUATION: \n--- 6 (Strong Accept) \n--- 5 (Accept) \n--- 4 (Weak Accept) \n--- 3 (Borderline Paper) \n--- 2 (Weak Reject) \n--- 1 (Reject) \n--- 0 (Strong Reject)\n" << std::endl;
	ofile << "*** REVIEWER'S CONFIDENCE:  from 1 (lowest) to 5 (highest) \n--- 5 (Expert) \n--- 4 (High) \n--- 3 (Medium) \n--- 2 (Low) \n--- 1 (Null)\n" << std::endl;
	ofile << "*** RELEVANCE: from 1 (lowest) to 5 (highest) \n--- 5 (Highly relevant) \n--- 4 (Very Relevant) \n--- 3 (Moderately Relevant) \n--- 2 (Marginally Relevant) \n--- 1 (Not Relevant)\n" << std::endl;
	ofile << "*** ORIGINALITY: from 1 (lowest) to 5 (highest) \n--- 5 (Highly Original) \n--- 4 (Very Original) \n--- 3 (Moderately Original) \n--- 2 (Marginally Original) \n--- 1 (Not Original)\n" << std::endl;
	ofile << "*** SIGNIFICANCE: from 1 (lowest) to 5 (highest) \n--- 5 (Highly Significant) \n--- 4 (Very Significant) \n--- 3 (Moderately Significant) \n--- 2 (Marginally Significant) \n--- 1 (Not Significant)\n" << std::endl;
	ofile << "*** PRESENTATION: from 1 (lowest) to 5 (highest) \n--- 5 (Excellent) \n--- 4 (Well Written) \n--- 3 (Acceptable) \n--- 2 (Poor) \n--- 1 (Unreadable)\n" << std::endl;
	ofile << "*** TECHNICAL QUALITY: from 1 (lowest) to 5 (highest) \n--- 5 (Technically Sound) \n--- 4 (Seems Sound) \n--- 3 (Minor Flaws) \n--- 2 (Major Flaws) \n--- 1 (Unsound)\n" << std::endl;
	ofile << "*** EVALUATION: from 1 (lowest) to 5 (highest) \n--- 5 (Thorough Evaluation) \n--- 4 (Strong Evaluation) \n--- 3 (Sound Evaluation) \n--- 2 (Weak Evaluation) \n--- 1 (No Evaluation)\n" << std::endl;
	ofile << "*** END\n" << linebreak;
	
	// close file
	ofile.close();
}
// generates a review from an uploaded form
Review generateReview(std::string fname)
{
	Review temp;
	temp.comments = "";
	
	// open the file
	std::ifstream ifile;
	ifile.open(fname.c_str(), std::iostream::in);
	// check the file
	if (ifile.is_open())
	{	
		std::cout << "\nReading review..." << std::endl;
		
		// get all the important lines
		std::string notifs = "";
		std::string next = "";
		std::string eval = "*** Evaluation: ";
		std::string rev = "*** Review: ";
		std::string pt = "*** Paper Title: ";
		std::string rb = "*** Reviewed By: ";
	
		// check it's good
		while (!ifile.eof())
		{
			// get the next line and add a delim
			std::getline(ifile, next);
			if (!ifile.eof())
			{
				if (next[0] == '*')
				{
					// get paper title
					if (next[4] == 'P')
					{
						if (next[10] != 's') // don't get paper's author
						{
							int pos = next.find_first_of(":");
							temp.paperName = next.substr(pos+2, next.length()-pos);
						}
					}
					// get the comments
					else if (strcmp(next.c_str(), rev.c_str()) == 0)
					{
						std::string reviews = "";
						std::getline(ifile, next); // instructions
						std::getline(ifile, next); // first line of comments
						do
						{
							reviews.append(next);
							reviews.append("\n");
							std::getline(ifile, next);
						}while (next[0] != '-');
						temp.reviews = reviews;
					}
					// get reviewer
					else if (next[4] == 'R')
					{
						int pos = next.find_first_of(":");
						temp.reviewName = next.substr(pos+2, next.length()-pos);
					}
					// get the scores
					else if (strcmp(next.c_str(), eval.c_str()) == 0)
					{
						// overall eval
						std::getline(ifile, next); // get rid of that line
						std::getline(ifile, next);
						std::getline(ifile, next);
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.overallEval = atof(next.c_str());

						// confidence
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.reviewConfidence = atoi(next.c_str());;

						// relevance
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.relevance = atoi(next.c_str());

						//originality
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.originality = atoi(next.c_str());

						// significance
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.significance = atoi(next.c_str());

						// presentation
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.presentation = atoi(next.c_str());

						// tech quality
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.techQuality = atoi(next.c_str());

						//evaluation
						while (next[0] != '*')
						{
							std::getline(ifile, next);
						}
						std::getline(ifile, next);
						while (next[0] == '-')
						{
							std::getline(ifile, next);
						}
						temp.eval = atoi(next.c_str());
					}
				}
			}
		}
		
		// get the average score
		temp.generateAvScore();
	}
	else
	{
		std::cout << "\nError: could not open the review. Please check the filename\n" << std::endl;
		temp.avScore = 100; // error value to be checked
	}
	
	ifile.close();
	return temp;
	
}
// opens a review file and turns it into a string
std::string getReview(std::string fname)
{
	std::ifstream ifile;
	ifile.open(fname.c_str(), std::ios::in);
	std::string res = "";
	
	if (ifile.is_open())
	{
		std::string next;
		
		while (!ifile.eof())
		{
			getline(ifile, next);
			if (!ifile.eof())
				res.append(next);
		}
	}
	
	return res;
}


// functions that connect to the server:
// gets all average scores of a paper
float avScoreOfPaper(sf::TcpSocket& socket, std::string ID, std::string papername)
{
	// get all the reviews of a paper
	std::vector<Review> allrevs = getAllReviews(socket, ID, papername);
	int size = 0;
	float score = 0;
	
	// loop through all reviews and update the score
	for (int i = 0; i < allrevs.size(); i++)
	{
		size++;
		score += allrevs[i].avScore;
	}
	
	// get the average and return
	score /= size;
	return score;
}
// submits a review
void submitReview(sf::TcpSocket& socket, std::string ID, std::string username, std::string papername)
{
	// get filename from user
	std::cout << "Enter filename: ";
	std::string fname;
	std::cin >> fname;
	
	// load a review from the filename
	Review temp = generateReview(fname);
	
	// if the review is valid 
	if (temp.avScore != 100)
	{
		std::cout << "Thank you for reviewing " << papername << "\n\n";
		
		// get a string version
		std::string review = temp.toString();
		
		// create a filename
		std::string fname = papername + "_reviews.txt";
		
		// send it to the server
		PacketType ptype = SET_REVIEW;
		sf::Packet packet;
		packet << ID << ptype << fname << username << review;
		socket.send(packet);
	}
	
}
// gets all reviews for a paper from the server
std::vector<Review> getAllReviews(sf::TcpSocket& socket, std::string ID, std::string papername)
{
	// send request to server
	std::string fname = papername + "_reviews.txt";
	std::string bigString;
	std::vector<Review> reviews;
	std::string review;
	PacketType pType = GET_REVIEWS;
	
	sf::Packet reviewsPacket;
	reviewsPacket << ID << pType << fname;
	socket.send(reviewsPacket);
	
	// handle response from server
	sf::Packet serverReply;
	if(socket.receive(serverReply) == sf::Socket::Done)
	{
		serverReply >> pType;
		if (pType == GET_REVIEWS)
		{
			serverReply >> bigString;
			bool done = false;
			// cuts it into review strings and turns those strings into reviews
			while (!done)
			{
				int start = bigString.find_first_of("~");
				// if there's another review to get
				if (start != std::string::npos)
				{
					int end = bigString.find_first_of("~", start+1);
					review = bigString.substr(start, end);
					bigString = bigString.substr(end+1, bigString.length()-end);

					Review temp;
					temp.fromString(review);
					reviews.push_back(temp);
				
				}
				else
					done = true;
			}
		}
	}
	
	return reviews;
}
// gets a comment from a user and appends it to the review
void addComment(sf::TcpSocket& socket, std::string ID, std::string username, std::string papername, std::vector<Review> reviews)
{
	//Create Packet
	PacketType pType = SET_COMMENT;
	sf::Packet commentPacket;
	
	// send data to server
	std::string fname = papername + "_reviews.txt";
	std::string allrevs = "";
	commentPacket << ID << pType << fname;
	for (int i = 0; i < reviews.size(); i++)
	{
		allrevs.append(reviews[i].toString());
	}
	commentPacket << allrevs;
	socket.send(commentPacket);
}
