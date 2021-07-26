#include<iostream>
#include<SFML/Graphics.hpp>
#include"Board.h"
#include"System.h"


using namespace std;




int main() {

	System* system = new System(1600, 1200);

	system->run();

	delete system;
	
	cin.get();
	return 0;
}