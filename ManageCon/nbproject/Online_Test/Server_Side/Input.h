#ifndef _INPUT_H
#define _INPUT_H
#include <SFML/Window/Keyboard.hpp>

class InputCtrl
{
/*
	Input Keys:
	Movement:
	Move up 	|	Up Arrow, W
	Move right	|	Right Arrow, D
	Move down 	| 	Down Arrow, S
	Move left 	| 	Left Arrow, A
	Interaction:
	Activate/Use	|	E
	ADMIN:
	Change Tile	|	C
*/
	private:
		int lastMove;
		double mytime;
		double hold;
		
		void Accelerate();
		
	public:
		InputCtrl() { lastMove = 0; mytime = 0; }
		
		int GetAction(double);
		int GetLastMove();
		
};
#endif
