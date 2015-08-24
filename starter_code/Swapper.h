#ifndef SWAPPER_H
#define	SWAPPER_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
using namespace std;

public double getPaperValue(SessionOrganizer* sorg, int paper);

public void swapPapers(SessionOrganizer* sorg, int paper1, int paper2);

// This function doesn't actually swap the papers.
public void getPaperSwapDiff(SessionOrganizer* sorg, int paper1, int paper2);

public double getSessionValue(SessionOrganizer* sorg, int track, int session);

public void swapSessions(SessionOrganizer* sorg, int track1, int session1, int track2, int session2);

// This function doesn't actually swap the sessions.
public void getSessionSwapDiff(SessionOrganizer* sorg, int track1, int session1, int track2, int session2);

#endif	/* SWAPPER_H */
