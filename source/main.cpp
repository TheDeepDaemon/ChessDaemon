#include<iostream>
#include<SFML/Graphics.hpp>
#include"Board.h"
#include"System.h"


using namespace std;







int main() {

	System* system = new System(800, 600);
	
	system->run();

	cin.get();
	delete system;

	return 0;
}