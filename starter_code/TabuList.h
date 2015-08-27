#ifndef TABULIST_H
#define	TABULIST_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

using namespace std;

struct Tabu{
	char* val;
	Tabu* next;
};

void initTabuSize(int size);

void enqueueTabu(char* str);

bool checkTabu(char* str);

void dequeueTabu();

int getTabuSize();


#endif	/* TABULIST_H */
