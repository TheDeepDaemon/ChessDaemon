#include<iostream>
#include<SFML/Graphics.hpp>
#include"Board.h"
#include"System.h"


using namespace std;




void runSystem() {
	System* system = new System(1600, 1200);

	system->run();

	cin.get();
	delete system;
}


int main() {


	
	cin.get();
	return 0;
}