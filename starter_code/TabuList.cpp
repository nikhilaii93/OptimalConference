#include "TabuList.h"

Tabu* headTabu;
Tabu* lastTabu;
int TABU_LIST_SIZE;
int CURR_TABU_SIZE;

void initTabuSize(int size)
{
	TABU_LIST_SIZE = size;
}

void enqueueTabu(char* str)
{
	if (!checkTabu(str) && TABU_LIST_SIZE > 0)
	{
		if (CURR_TABU_SIZE == 0)
		{
			headTabu = (Tabu *)malloc(sizeof(Tabu));
			headTabu->next = NULL;
			headTabu->val = (char *)malloc(strlen(str)+1);
			strcpy(headTabu->val,str);
			lastTabu = headTabu;
		}
		else
		{
			if (CURR_TABU_SIZE == TABU_LIST_SIZE)
			{
				dequeueTabu();
			}
			Tabu* currTabu = new Tabu;
			currTabu->next = NULL;
			currTabu->val = str;
			lastTabu->next = currTabu;
			lastTabu = currTabu;
		}
		CURR_TABU_SIZE++;
	}
}

bool checkTabu(char* str)
{
	Tabu* curr = headTabu;
	for (int i = 0; i < CURR_TABU_SIZE; i++)
	{
		if (!strcmp(curr->val,str))
		{
			return true;
		}
		curr = curr->next;
	}
	return false;
}

void dequeueTabu()
{
	if (CURR_TABU_SIZE == TABU_LIST_SIZE && headTabu != NULL)
	{
		Tabu* toDel = headTabu;
		headTabu = headTabu->next;
		CURR_TABU_SIZE--;
		free(toDel->val);
		free(toDel);
	}
}


