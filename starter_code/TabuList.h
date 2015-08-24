#ifndef TABULIST_H
#define	TABULIST_H

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

struct TabuList{
	string val;
	TabuList* next;
};

TabuList* headTabu = NULL;
TabuList* lastTabu = NULL;
int TABU_LIST_SIZE = 0;
int CURR_TABU_SIZE = 0;

public void initTabuSize(int size);

public void enqueueTabu(SessionOrganizer* sorg);

public bool checkTabu(SessionOrganizer* sorg);

public void dequeueTabu();

public string solToStr(SessionOrganizer* sorg);

#endif	/* TABULIST_H */
