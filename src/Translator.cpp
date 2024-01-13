#include <Translator.hpp>
#include <fstream>
#include <iostream>

#define UKRAINIAN_TRANSLATION_FILENAME "./translation/ukr.txt"


Translator::Translator() {
}

Translator::~Translator() {
}

void Translator::loadTranslation() {
	std::ifstream in(std::string(UKRAINIAN_TRANSLATION_FILENAME));
	if (not in) {
		std::cerr << "Failed to open " << UKRAINIAN_TRANSLATION_FILENAME
				  << std::endl;
		return;
	}
	while (in) {
		std::string eng, ukr;
		std::getline(in, eng);
		std::getline(in, ukr);
		mTranslation[UKRAINIAN_LANGUAGE][eng] = ukr;
	}
	in.close();
}

std::string Translator::translate(const std::string& s, Language lang) {
	if (lang == ENGLISH_LANGUAGE) {
		return s;
	}
	if (mTranslation[lang][s] == std::string()) {
		std::cerr << "Failed to translate: " << s << std::endl;
	}
	return mTranslation[lang][s];
}

