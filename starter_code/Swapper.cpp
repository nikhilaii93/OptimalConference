#include "Swapper.h"

public double getPaperValue(SesssionOrganizer* sorg, int paper)
{
    double value1 = 0;
    double value2 = 0;
    Track currTrack;
    Session currSession;

    Conference *conference = sorg->conference;
    double tradeoffCoefficient = sorg->tradeoffCoefficient;
    double** distMat = sorg->getDistanceMatrix();
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

    // Get value of differrence
    for (int i = 0; i < conference->getTracksInSession(); i++)
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

public void swapPapers(SesssionOrganizer* sorg, int paper1, int paper2)
{
	Conference *conference = sorg->conference;
	int trackId1 = conference->paperInfo[paper1].first;
	int trackId2 = conference->paperInfo[paper2].first;
	int sessionId1 = conference->paperInfo[paper1].second;
	int sessionId2 = conference->paperInfo[paper2].second;

	Track track1 = conference->getTrack(trackId1);
	Track track2 = conference->getTrack(trackId2);
	Session session1 = track1.getSession(sessionId1);
	Session session2 = track1.getSession(sessionId2);

	for (int i = 0; i < conference->getPapersInSession(); i++)
	{
		if (session1.getPaper(i) = paper1)
		{
			break;
		}
	}
	for (int j = 0; j < conference->getPapersInSession(); j++)
	{
		if (session2.getPaper(j) = paper2)
		{
			break;
		}
	}
	// Swap papers in conference and in paperInfo
	conference->setPaper(trackId1, sessionId1, i, paper2);
	pair paper2Info = make_pair(trackId1, session1);
	conference->paperInfo[paper2] = paper2Info;

	conference->setPaper(trackId2, sessionId2, j, paper1);
	pair paper1Info = make_pair(trackId2, session2);
	conference->paperInfo[paper1] = paper1Info;

	return;
}

public double getPaperSwapDiff(SessionOrganizer* sorg, int paper1, int paper2)
{
	double diff = 0.0;

	swapPapers(sorg, paper1, paper2);

	diff += getPaperValue(sorg, paper1);
	diff += getPaperValue(sorg, paper2);

	swapPapers(sorg, paper1, paper2);

	diff -= getPaperValue(sorg, paper1);
	diff -= getPaperValue(sorg, paper2);

	return diff;
}

public double getSessionValue(SessionOrganizer* sorg, int track, int session)
{
	double value = 0.0;
	double** distMat = sorg->getDistanceMatrix();
	Session currSession = conference->getTrack(track).getSession(session); 
	for (int i = 0; i < conference->getParallelTracks(); i++)
	{
		if (i != track)
		{
			Session otherSession = conference->getTrack(i).getSession(session);
			for (int j = 0; j < conference->getPapersInSession(); j++)
			{
				for (int k = 0; < conference->getPapersInSession(); k++)
				{
					value += conference->distMat[currSession.getPaper(j)][otherSession.getPaper(k)];
				}
			}
		}
	}
	return value;
}

public void swapSessions(SessionOrganizer* sorg, int track1, int session1, int track2, int session2)
{
	Session s1 = sorg->conference->getTrack(track1).getSession(session1);
	Session s2 = sorg->conference->getTrack(track2).getSession(session2);
	for (int i = 0; i < sorg->conference->getPapersInSession(); i++)
	{
		swapPapers(sorg, s1.getPaper(i), s2.getPaper(i));
	}
	return;
}

// This function doesn't actually swap the sessions.
public double getSessionSwapDiff(SessionOrganizer* sorg, int track1, int session1, int track2, int session2)
{
	double diff = 0.0;

	swapSessions(sorg, track1, session1, track2, session2);

	diff += getSessionValue(sorg, track1, session1);
	diff += getSessionValue(sorg, track2, session2);

	swapSessions(sorg, track1, session1, track2, session2);

	diff -= getPaperValue(sorg, track1, session1);
	diff -= getPaperValue(sorg, track2, session2);

	return diff;
}
