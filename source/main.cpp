#include"System.h"






int main() {
	srand((unsigned)time(NULL));

	System* system = new System("ChessDaemon/images");

	system->run();

	delete system;

	cout << "Press enter to continue...\n";
	cin.get();
	return 0;
}
