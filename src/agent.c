#include "u.h"

Agent *newagent(void)
{
	Agent *a;

	a = malloc(sizeof(Agent));
	return a;	
}

void agentinit(void)
{
	newagent();
}

void agentrun(void)
{
	
}
