#ifndef COLOUR_H
#define COLOUR_H
#include<iostream>

namespace text
{
	// Colours
	enum Colour
	{
		Colour_Black = 0,
		Colour_Red = 1,
		Colour_Green = 2,
		Colour_Yellow = 3,
		Colour_Blue = 4,
		Colour_Purple = 5,
		Colour_Cyan = 6,
		Colour_White = 7
	};
	
	// Effects
	enum Effect
	{
		Effect_Bold = 1,
		Effect_Underline = 4,
		Effect_Intense = 9,
		Effect_None = 0
	};
	
	// Backgrounds
	enum Background
	{
		Bkg_Black = 0,
		Bkg_Red = 1,
		Bkg_Green = 2,
		Bkg_Yellow = 3,
		Bkg_Blue = 4,
		Bkg_Purple = 5,
		Bkg_Cyan = 6,
		Bkg_White = 7,
		Bkg_None = 8
	};
	
	std::string styleString(const std::string & str, Colour col = Colour_White, Effect eft = Effect_None, Background bkg = Bkg_None);
	
	std::string styleString(const std::string & str, Colour col, Effect eft, Background bkg)
	{
		std::ostringstream os;
		os << "\033[";
		
		//apply effect
		if(eft != text::Effect_Intense)
		{
			os << eft << ';';
			os << '3';
		}
		else if(eft == text::Effect_Intense)
		{
			os << "0;" << eft;
		}
		//apply colour
		os << col << 'm';
		//apply background 
		if(bkg != text::Bkg_None)
		{
			os << "\033[4" << bkg << "m";
		}
		//insertstring
		os << str << "\033[0m";
		return os.str();
	}
};

#endif
