#include<iostream>
#include<SFML/Graphics.hpp>
#include"Board.h"
#include"System.h"


using namespace std;




int main() {

	System::initInstance(1600, 1200);
	System* system = System::getInstance();

	system->run();

	delete system;
	
	cin.get();
	return 0;
}