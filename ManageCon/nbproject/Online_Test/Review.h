#ifndef REVIEW_H
#define	REVIEW_H

#include <string>

class Review
{
public:
	Review() {}
	~Review() {}
	
	// functions
	void display();
	void generateAvScore();
	std::string toString();
	void fromString(std::string);

	// metadata
	std::string reviewName;
	std::string paperName;
	
	// total score
	float avScore;
	
	// reviews
	std::string reviews; // the comments IN the review
	std::string comments; // the comments ON the review
	
	// scores
	int overallEval;
	int reviewConfidence;
	int relevance;
	int originality;
	int significance;
	int presentation;
	int techQuality;
	int eval;
};

// deal with the Review class, but aren't tech. part of it
void generateForm(std::string);
Review generateReview(std::string);
std::string getReview(std::string);
void submitReview(sf::TcpSocket&, std::string, std::string, std::string);
std::vector<Review> getAllReviews(sf::TcpSocket&, std::string, std::string);
void addComment(sf::TcpSocket&, std::string, std::string, std::string, std::vector<Review>);

#endif