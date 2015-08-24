#include "TabuList.h"

public void initTabuSize(int size)
{
	TABU_LIST_SIZE = size;
}

public void enqueueTabu(SessionOrganizer* sorg)
{
	string str = solToStr(sorg);
	if (!checkTabu(str) && TABU_LIST_SIZE > 0)
	{
		if (headTabu == NULL)
		{
			headTabu = new TabuList;
			headTabu->next = NULL;
			headTabu->val = solToStr(sorg);
			lastTabu = headTabu;
		}
		else
		{
			if (CURR_TABU_SIZE == TABU_LIST_SIZE)
			{
				dequeueTabu();
			}
			TabuList* currTabu = new TabuList;
			currTabu->next = NULL;
			currTabu->val = str;
			lastTabu->next = currTabu;
			lastTabu = currTabu;
		}
		CURR_TABU_SIZE++;
	}
}

public bool checkTabu(string str)
{
	string str = solToStr(sorg);
	TabuList* curr = headTabu;
	for (int i = 0; i < CURR_TABU_SIZE; i++) {
		if (curr.val == str) {
			return true;
		}
		curr = curr->next;
	}
	return false;
}

public void dequeueTabu()
{
	if (CURR_TABU_SIZE == TABU_LIST_SIZE && headTabu != NULL)
	{
		TabuList* toDel = headTabu;
		headTabu = headTabu->next;
		CURR_TABU_SIZE--;
		free(toDel);
	}
}

public string solToStr(SessionOrganizer* sorg)
{
	string str = "";
	for ( int i = 0; i < sorg->conference->getSessionsInTrack(); i++ )
    {
        for ( int j = 0; j < sorg->conference->getParallelTracks(); j++ )
        {
            for ( int k = 0; k < sorg->conference->getPapersInSession(); k++ )
            {
                str += conference->tracks[j].getSession(i).getPaper(k);
                str += " ";
            }
            if (j != sorg->conference->getParallelTracks() - 1)
            {
                str += "| ";
            }
        }
        str += "\n";
    }
    return str;
}
