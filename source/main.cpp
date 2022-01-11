#include"headers.h"
#include"ChessBoard.h"
#include"System.h"


using namespace std;



int main() {

	System::initInstance();
	System* system = System::getInstance();

	system->run();

	delete system;
	
	cin.get();
	return 0;
}