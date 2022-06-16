#include"headers.h"
#include"ChessBoard.h"
#include"System.h"


using std::cout;
using std::cin;



int main() {

	System::initInstance();
	System* system = System::getInstance();

	system->run();

	delete system;
	
	cin.get();
	return 0;
}