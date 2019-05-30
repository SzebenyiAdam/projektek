#include "Utilities.h"



Utilities::Utilities()
{
}

double Utilities::calcElapsedTime()
{
	int current_time;
	double elapsed_secs;

	current_time = glutGet(GLUT_ELAPSED_TIME);
	elapsed_secs = (double)(current_time - time) / 1000.0;
	time = current_time;

	return elapsed_secs;
}
