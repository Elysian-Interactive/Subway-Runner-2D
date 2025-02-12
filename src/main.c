#include <Game.h>

int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	return r;
error:
	return -1;
}
