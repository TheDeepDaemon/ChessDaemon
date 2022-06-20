#include"System.h"
#include<random>
#include<algorithm>
#include"GameTree.h"
#include<string>


using std::vector;



void runSystem() {
	System* system = new System("ChessDaemon/images");

	system->run();

	delete system;
}




int main() {

	runSystem();


	cout << "Press enter to continue...";
	cin.get();
	return 0;
}
