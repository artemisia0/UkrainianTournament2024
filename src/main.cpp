
#include <App.hpp>
#include <iostream>
#include <stdexcept>


int main() {  // Original author of this application is artemisia0 (github).
	std::string itsMe("Programmer is Artem Protas. My github is artemisia0");
	std::cout << itsMe << std::endl;
	
	try {
		App app;
		app.run();
		return 0;
	} catch (std::exception& err) {
		std::cerr << err.what() << std::endl;
		return -1;
	} catch (...) {
		std::cerr << "Unknown exception thrown" << std::endl;
		return -2;
	}
}
