#include "Input.h"

int InputCtrl::GetLastMove()
{
	return lastMove;
}

int InputCtrl::GetAction(double deltaTime)
{	
       	if(mytime < 0.2)
       	{
       		mytime += deltaTime;
       		hold += deltaTime;
       	}
       	else
       	{
       		hold = 0;
       	}
       	if(mytime >= 0.12 || hold > 1)
       	{
       		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
       		{  
       			return 1024;
       		}
       		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       		{      	 		
      	 		mytime = 0;
      	 		Accelerate();
      	 		lastMove = 1;
      	 		return 1;
       		}
     	  	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
     	  	{
     	  		mytime = 0;
     	  		Accelerate();
     	  		lastMove = 3;
     	  		return 3;
     	  	}
    	   	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    	   	{
    	   		mytime = 0;
    	   		Accelerate();
    	   		lastMove = 0;
    	   		return 0;
   	    	}
       		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
       		{       			
       			mytime = 0;
       			Accelerate();
       			lastMove = 2;
       			return 2;
      	 	}
      	 	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
      	 	{
      	 		mytime = 0;
      	 		hold = 0;
      	 		return 4;
      	 	}
      	 	if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
      	 	{
      	 		mytime = 0;
      	 		hold = 0;
      	 		return 42;
      	 	}
      	 }
      	 return -1;
}


void InputCtrl::Accelerate()
{
	if(hold > 1)
      	{
      		hold = 0.97;
      	}
}
