#include "Swapper.h"

#define DEBUG_SWAPPER 0

double getPaperValue(Conference* conference, int paper, double** distMat, double tradeoffCoefficient)
{
    double value1 = 0;
    double value2 = 0;
    Track currTrack;
    Session currSession;

    // Get track and session info from paperId.
    int trackId = conference->paperInfo[paper].first;
    int sessionId = conference->paperInfo[paper].second;

    // Get value of similarity.
    currTrack = conference->getTrack(trackId);
    currSession = currTrack.getSession(sessionId);
    for (int i = 0; i < conference->getPapersInSession(); i++)
    {
    	int otherId = currSession.getPaper(i);
        if (otherId != paper)
        {
            value1 += 1 - distMat[paper][otherId];
        }
    }

    // Get value of difference
    for (int i = 0; i < conference->getParallelTracks(); i++)
    {
        if (i != trackId)
        {
            currTrack = conference->getTrack(i);
    		currSession = currTrack.getSession(sessionId);
    		for (int j = 0; j < conference->getPapersInSession(); j++)
    		{
	            value2 += distMat[paper][currSession.getPaper(j)];
	        }
        }
    }
    return value1 + tradeoffCoefficient * value2;
}

void swapPapers(Conference* conference, int paper1, int paper2)
{
	
	if (DEBUG_SWAPPER) 
		cout << "swpP1" << endl;
	
	int trackId1 = conference->paperInfo[paper1].first;
	int trackId2 = conference->paperInfo[paper2].first;
	int sessionId1 = conference->paperInfo[paper1].second;
	int sessionId2 = conference->paperInfo[paper2].second;
	
	if (DEBUG_SWAPPER)
		cout << "swpP2" << endl;
	
	Track track1 = conference->getTrack(trackId1);
	Track track2 = conference->getTrack(trackId2);
	Session session1 = track1.getSession(sessionId1);
	Session session2 = track2.getSession(sessionId2);
	
	if (DEBUG_SWAPPER)
		cout << "swpP3" << endl;

	int ik = -1;
	for (int i = 0; i < conference->getPapersInSession(); i++)
	{
		if (DEBUG_SWAPPER)
			cout << "swpP4" << endl;
		
		if (session1.getPaper(i) == paper1)
		{
			ik = i;
			break;
		}
	}

	int jk = -1;
	for (int j = 0; j < conference->getPapersInSession(); j++)
	{
		if (DEBUG_SWAPPER)
			cout << "swpP5 " << j << " " << session2.getPaper(j) << " " << paper2 << endl;
		
		if (session2.getPaper(j) == paper2)
		{
			jk = j;
			break;
		}
	}
	
	if (DEBUG_SWAPPER)
		cout << "swpP6" << endl;
	
	// Swap papers in conference and in paperInfo
	conference->setPaper(trackId1, sessionId1, ik, paper2);
	pair<int,int> paper2Info = make_pair(trackId1, sessionId1);
	conference->paperInfo[paper2] = paper2Info;
	// printPaperInfo(conference->paperInfo, 1);

	if (DEBUG_SWAPPER)
		cout << "swpP7" << endl;
	
	conference->setPaper(trackId2, sessionId2, jk, paper1);
	pair<int,int> paper1Info = make_pair(trackId2, sessionId2);
	conference->paperInfo[paper1] = paper1Info;
	// printPaperInfo(conference->paperInfo, 2);
	
	if (DEBUG_SWAPPER)
		cout << "swpP8" << endl;
	
	return;
}

double getPaperSwapDiff(Conference* conference, int paper1, int paper2, double** distMat, double tCoeff)
{
	double diff = 0.0;
	
	if (DEBUG_SWAPPER)
		cout << "gPSD1" << endl;
	
	swapPapers(conference, paper1, paper2);
	
	if (DEBUG_SWAPPER)
		cout << "gPSD2" << endl;
	
	diff += getPaperValue(conference, paper1, distMat, tCoeff);
	diff += getPaperValue(conference, paper2, distMat, tCoeff);
	
	if (DEBUG_SWAPPER)
		cout << "gPSD3" << endl;
	
	swapPapers(conference, paper1, paper2);
	
	if (DEBUG_SWAPPER)
		cout << "gPSD4" << endl;
	
	diff -= getPaperValue(conference, paper1, distMat, tCoeff);
	diff -= getPaperValue(conference, paper2, distMat, tCoeff);

	return diff;
}

double getSessionValue(Conference* conference, int track, int session, double** distMat)
{
	double value = 0.0;
	Session currSession = conference->getTrack(track).getSession(session);
	for (int i = 0; i < conference->getParallelTracks(); i++)
	{
		if (i != track)
		{
			Session otherSession = conference->getTrack(i).getSession(session);
			for (int j = 0; j < conference->getPapersInSession(); j++)
			{
				for (int k = 0; k < conference->getPapersInSession(); k++)
				{
					value += distMat[currSession.getPaper(j)][otherSession.getPaper(k)];
				}
			}
		}
	}
	return value;
}

void swapSessions(Conference* conference, int track1, int session1, int track2, int session2)
{
	Session s1 = conference->getTrack(track1).getSession(session1);
	Session s2 = conference->getTrack(track2).getSession(session2);
	for (int i = 0; i < conference->getPapersInSession(); i++)
	{
		swapPapers(conference, s1.getPaper(i), s2.getPaper(i));
	}
	return;
}

// This function doesn't actually swap the sessions.
double getSessionSwapDiff(Conference* conference, int track1, int session1, int track2, int session2, double** distMat)
{
	double diff = 0.0;

	swapSessions(conference, track1, session1, track2, session2);

	diff += getSessionValue(conference, track1, session1, distMat);
	diff += getSessionValue(conference, track2, session2, distMat);

	swapSessions(conference, track1, session1, track2, session2);

	diff -= getSessionValue(conference, track1, session1, distMat);
	diff -= getSessionValue(conference, track2, session2, distMat);

	return diff;
}

void printPaperInfo(vector<pair<int,int> > info, int place)
{
	for (int i = 0; i < info.size(); i++)
	{
		cout << i << " " << info[i].first << " " << info[i].second << endl;
	}
	cout << endl;
}
