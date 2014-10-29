// CSCI222 Paper Assignment System

#ifndef ASSIGN_H
#define ASSIGN_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

const int maxKeywords = 5;

struct keywords {
	std::string name; // Reviewer/Author Name
	std::string words[maxKeywords]; // Keywords
	int numWords; // Number of keywords (max set by maxKeywords)
};

int getScore(std::string str1, std::string str2);

void AssignPaper(const std::string& paperFile, const std::string& reviewFile)
{
	int numPapers = 0;
	int numReviewers = 0;
	std::vector<keywords> papers;
	std::vector<keywords> reviewers;
	
// Get keywords from papers
	std::ifstream file;
	std::string temp;
	file.open(paperFile.c_str());
	
	if(file.good())
	{	
		while(std::getline(file, temp))
		{
			if(!file.eof())
			{
				numPapers++;
			}
		}
	}
	file.close();
	file.open(paperFile.c_str());
	//std::cout << "numPapers: " << numPapers << std::endl;
	std::string keyword;
	keywords tempKeys;

	for (int i = 0; i < numPapers; i++) 
	{
		//std::cout << "For, i=" << i << std::endl;
		std::getline(file, temp);
		std::istringstream tempStream(temp);
		int j = 0;
		tempStream >> tempKeys.name;
		//std::cout << "Name: " << tempKeys.name << std::endl;
		while (tempStream >> keyword) 
		{
			std::cout << "j[" << j << "] = " << keyword << std::endl;
			tempKeys.words[j] = keyword;
			j++;
		}
		tempKeys.numWords = j;
		//std::cout << "numWords: " << tempKeys.numWords << std::endl;
		papers.push_back(tempKeys);
	}
	file.close();

	
// Get keywords from reviewers
	file.open(reviewFile.c_str());
	
	if(file.good())
	{	
		while(std::getline(file, temp))
		{
			if(!file.eof())
			{
				numReviewers++;
			}
		}
	}
	file.close();
	file.open(reviewFile.c_str());
	//std::cout << "numPapers: " << numPapers << std::endl;
	//std::cout << "numReviewers: " << numReviewers << std::endl;
	
	for (int i = 0; i < numReviewers; i++) 
	{
		std::getline(file,temp,','); //get name
		tempKeys.name = temp;
		std::getline(file,temp,','); //get password
		std::getline(file,temp,','); //get email
		//std::cout << "Name: " << tempKeys.name << std::endl;
		std::getline(file,temp,','); //get number of keywords
		int max = atoi(temp.c_str());
		//std::cout << "temp: " << temp << " max" << max << std::endl;
		for(int j = 0; j < max; j++)
		{
			if(j < max-1)
			{
				std::getline(file,temp,',');
			}
			else
			{
				std::getline(file,temp,'\n');
			}
			keyword = temp;
			//std::cout << "j[" << j << "] = " << keyword << std::endl;
			tempKeys.words[j] = keyword;
		}
		tempKeys.numWords = max;
		//std::cout << "numWords: " << tempKeys.numWords << std::endl;
		reviewers.push_back(tempKeys);
	}
	
	
// Calculate 'score' for each match
	int* scores = new int[numPapers*numReviewers]; // Score matrix, access using scores[y*numReviewers + x]
	for(int i = 0; i < numPapers*numReviewers; i++)
	{
		scores[i] = 0;
	}	
	for (int i = 0; i < numPapers; i++) 
	{
		for (int j = 0; j < numReviewers; j++) 
		{
			for (int k = 0; k < papers[i].numWords; k++) 
			{
				int score = 0;
				for (int l = 0; l < reviewers[j].numWords; l++) 
				{
					score += getScore(papers[i].words[k], reviewers[j].words[l]);
				}
				scores[(i*numReviewers)+j] += score;
			}
		}
	}
	// Test output
	/*std::cout << "Testing reviewer07:" << std::endl;
	std::cout << reviewers[6].words[0] << reviewers[6].words[1] << reviewers[6].words[2] << std::endl;
	for (int i = 0; i < numPapers; i++) 
	{
		std::cout << "Paper" << i << " Score: " << scores[(6*numPapers)+i] << std::endl;
	}*/

	
// Assign papers by score
	int* assigned = new int[numPapers];
	for (int i = 0; i < numPapers; i++) 
	{
		assigned[i] = -1; // -1 = not assigned, else number indicates reviewer assigned
	}

	int best = 0;
	int selectedReviewer = 0;
	for (int i = 0; i < numPapers; i++)
	{
		best = 0;
		selectedReviewer = 0;
		for (int j = 0; j < numReviewers; j++) 
		{
			if(i == 0)
			{
				//std::cout << "REVIEWER = " << j  << " with " << scores[(i*numReviewers)+j] << std::endl;
			}
			if(scores[(i*numReviewers)+j] > best)
			{
				best = scores[(i*numReviewers)+j];
				if(i == 0)
				{
					//std::cout << "NEW BEST = " << j  << " with " << best << std::endl;
				}
				selectedReviewer = j;
			}
		}
		assigned[i] = selectedReviewer;
	}
	
// Assignment Result
	int a;
	for (int i = 0; i < numPapers; i++) 
	{
		a = assigned[i];
		//std::cout << "a: " << a << std::endl;
		std::cout << papers[i].name << " assigned to " << reviewers[a].name << std::endl;
	}
	
	delete[] assigned;
	delete[] scores;
}

int getScore(std::string str1, std::string str2) 
{
	//std::cout << "Comparing str1: " << str1 << ", str2: " << str2 << std::endl;
	if (str1 == "" || str2 == "") // Either string empty gives score of 0
	{ 
		return 0;
	}
	
	if (str1 == str2) // Direct match gives score of 2
	{ 
		return 2;
	}
	
	std::size_t found = str1.find(str2); // Contains string, score 1
	if (found != std::string::npos) 
	{
		return 1;
	}
	
	return 0;
}

#endif
