#ifndef SWAPPER_H
#define	SWAPPER_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include "Conference.h"

using namespace std;

double getPaperValue(Conference* conference, int paper, double** distMat, double tradeoffCoefficient);

void swapPapers(Conference* conference, int paper1, int paper2);

// This function doesn't actually swap the papers.
double getPaperSwapDiff(Conference* conference, int paper1, int paper2, double** distMat, double tCoeff);

double getSessionValue(Conference* conference, int track, int session, double** distMat);

void swapSessions(Conference* conference, int track1, int session1, int track2, int session2);

// This function doesn't actually swap the sessions.
double getSessionSwapDiff(Conference* conference, int track1, int session1, int track2, int session2, double** distMat);

#endif	/* SWAPPER_H */
