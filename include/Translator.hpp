#pragma once

#include <Language.hpp>
#include <string>
#include <map>


class Translator {
public:
	Translator();
	~Translator();
	
	void loadTranslation();

	std::string translate(const std::string&, Language);


private:
	std::map<Language, std::map<std::string, std::string>> mTranslation;
};
