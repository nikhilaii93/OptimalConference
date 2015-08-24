#include "Randomizer.h"

public vector<int> generateRandom(int n) {
    vector<int> rndSol(n);
    for (int i = 0; i < n; i++) {
    	rndSol[i] = i;
    }
    unsigned seed = std::chrono::system_clock::now.time_since_epoch().count();
    shuffle(rndSol.begin(), rndSol.end(), default_random_engine(seed));
    return rndSol;
}

public void setRandom(SessionOrganizer* sorg) {
	int pt = sorg->conference->getParallelTracks();
    int st = sorg->conference->getSessionsInTrack();
    int ps = sorg->conference->getPapersInSession();
    int n = pt*st*ps;
	vector<int> rndSol = generateRandom(n);

	int ijk	 = 0;
    for ( int i = 0; i < st; i++ )
    {
        for ( int j = 0; j < pt; j++ )
        {
            for ( int k = 0; k < ps; k++ )
            {
                sorg->conference->setPaper(j, i, k, rndSol[ij]);
                pair currP = make_pair(j,i);
                sorg->conference->paperInfo[rndSol[ij]] = currP;
                ijk++;
            }
        }
    }
}
