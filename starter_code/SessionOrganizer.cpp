/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"

#define DEBUG_SESSIONORGANIZER 0

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );

    int pt = conference->getParallelTracks();
	int st = conference->getSessionsInTrack();
	int ps = conference->getPapersInSession();
	int n = pt*st*ps;

	TABU_SIZE = 100;
	MIN_THRESHOLD = 0.005;
	MIN_IMPROVEMENT = MIN_THRESHOLD;
	DO_SHOULDER = false;
	ALLOWED_SHOULDER = 0;
	TOTAL_SHOULDER = 0;
	CURR_SHOULDER = 0;
	DO_BAD_MOVES = false;
	ALLOWED_BAD_MOVES = 0;
	TOTAL_BAD_MOVES = 0;
	BAD_MOVE_THRESHOLD =  11;

	BEST_SCORE = 0.0;
	CURR_SCORE = 0.0;
	BEST_SOL = (char *)malloc(totalDigitsTill(n)*4);
}

class compare
{
public:
    compare(){};
    bool operator() (const pair<pair<int,int>,double>& s1, const pair<pair<int,int>,double>& s2)
    {
        return (s1.second < s2.second);
    }
};

vector<pair<pair<int,int>,double> > SessionOrganizer::getSwaps()
{
	
    if (DEBUG_SESSIONORGANIZER)
        cout << "gSwp1" << endl;

    int pt = conference->getParallelTracks();
    int st = conference->getSessionsInTrack();
    int ps = conference->getPapersInSession();
    int n = pt*st*ps;
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "gSwp2" << endl;

    vector<pair<pair<int,int>,double> > possSwaps(1000);
    int totalSwaps = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {	
            if (DEBUG_SESSIONORGANIZER)
            	cout << "gSwp21" << endl;

            double diff = getPaperSwapDiff(conference, i, j, distanceMatrix, tradeoffCoefficient);
            
            if (DEBUG_SESSIONORGANIZER)
                cout << "gSwp22" << endl;

            if (diff >= MIN_IMPROVEMENT
                || (DO_SHOULDER && diff >= 0)
                || DO_BAD_MOVES)
            {
            	
                if (DEBUG_SESSIONORGANIZER)
                    cout << "gSwp3" << endl;

                pair<int,int> currP = make_pair(i,j);
                if (DEBUG_SESSIONORGANIZER)
                	cout << "gSwp31" << endl;
                pair<pair<int,int>,double> currS = make_pair(currP,diff);
                if (DEBUG_SESSIONORGANIZER)
                	cout << "gSwp32" << endl;
                if (totalSwaps == possSwaps.size()-10) {
                	possSwaps.resize(possSwaps.size()*2);
                }
                possSwaps.push_back(currS);
                totalSwaps++;

                if (DEBUG_SESSIONORGANIZER)
                	cout << "gSwp4" << endl;
            }
        }
    }
    return possSwaps;
}

pair<pair<int,int>,double> SessionOrganizer::getValidSwap(vector<pair<pair<int,int>,double> > possSwaps)
{   
	pair<int,int> dummy = make_pair(-1,-1);
    pair<pair<int,int>,double> validSwap = make_pair(dummy,0.0);
    for (int i = possSwaps.size()-1; i >= 0 ; i--)
    {
        pair<pair<int,int>,double> possSwap = possSwaps[i];
		if (TABU_SIZE > 0)
		{
			swapPapers(conference, possSwap.first.first, possSwap.first.second);
			// Must be freed
			char* possStr = solToStr();
			swapPapers(conference, possSwap.first.first, possSwap.first.second);
			if (checkTabu(possStr))
			{
				free(possStr);
				continue;
			}
		}

		if (possSwap.second < MIN_IMPROVEMENT)
		{
			break;
		}

		validSwap = possSwap;
		break;
    }
    return validSwap;
}

bool SessionOrganizer::doSomething()
{   
	
    if (DEBUG_SESSIONORGANIZER)
        cout << "dSmg1" << endl;

    srand (time(NULL));
    DO_BAD_MOVES = (rand()%10 + 1 > BAD_MOVE_THRESHOLD);
    DO_SHOULDER = (CURR_SHOULDER < ALLOWED_SHOULDER);
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "dSmg12" << endl;

    vector<pair<pair<int,int>,double> > possSwaps = getSwaps();
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "dSmg2" << endl;

    bool DO_RANDOM_SWAPS = false;
    if (rand()%10 + 1 > BAD_MOVE_THRESHOLD)
    {
    	
    	if (DEBUG_SESSIONORGANIZER)
            cout << "dSmg3" << endl;

        random_shuffle(possSwaps.begin(), possSwaps.end());
        DO_RANDOM_SWAPS = true;
    }
    else
    {
    	
        if (DEBUG_SESSIONORGANIZER)
            cout << "dSmg4" << endl;

        sort(possSwaps.begin(), possSwaps.end(), compare());
    }

    MIN_IMPROVEMENT = MIN_THRESHOLD;
    if (CURR_SHOULDER < ALLOWED_SHOULDER)
    {
    MIN_IMPROVEMENT = 0;
    }
    if (DO_RANDOM_SWAPS)
    {
        MIN_IMPROVEMENT = DBL_MIN;
    }
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "dSmg5" << endl;

    pair<pair<int,int>,double> selSwap = getValidSwap(possSwaps);
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "dSmg6" << endl;

    if (!(selSwap.first.first == -1 && selSwap.first.second == -1))
    {
    	
        if (DEBUG_SESSIONORGANIZER)
            cout << "dSmg7" << endl;

        if (!DO_RANDOM_SWAPS && selSwap.second >= MIN_THRESHOLD)
        {
            CURR_SHOULDER = 0;
        }
        else if (!DO_RANDOM_SWAPS
                && selSwap.second >= 0 && CURR_SHOULDER < ALLOWED_SHOULDER)
        {
            CURR_SHOULDER++;
            TOTAL_SHOULDER++;
        }
        
        if (DEBUG_SESSIONORGANIZER)
            cout << "dSmg8" << endl;

        swapPapers(conference, selSwap.first.first, selSwap.first.second);

        double score = CURR_SCORE + selSwap.second;
        
        if (DEBUG_SESSIONORGANIZER)
            cout << "dSmg9" << endl;

        // Will be freed on dequeue
        char* solStr = solToStr();
        enqueueTabu(solStr);
        CURR_SCORE = score;
        if (BEST_SCORE < scoreOrganization() /* CURR_SCORE > BEST_SCORE */) {
        	BEST_SCORE = scoreOrganization();
        	cout << "BEST_SCORE: " << BEST_SCORE << endl;
            // BEST_SCORE = CURR_SCORE;
            strcpy(BEST_SOL,solStr);
        }
        free(solStr);

        if (DEBUG_SESSIONORGANIZER)
        	cout << "dSmg10" << endl;

        return true;
    } else {
        return false;
    }
}

void SessionOrganizer::organizePapers()
{   
    if (DEBUG_SESSIONORGANIZER)
        cout << "orgP1" << endl;

	initTabuSize(TABU_SIZE);
	long start = 0, end = 0;
    start = time(0);
    
    if (DEBUG_SESSIONORGANIZER)
        cout << "orgP2" << endl;

    setRandom();
    while(processingTimeInMinutes*60-10 > end-start) {
    	
        if (DEBUG_SESSIONORGANIZER)
            cout << "orgP3" << endl;

        if (!doSomething()) {
            if (DEBUG_SESSIONORGANIZER)
                cout << "orgP4" << endl;

            setRandom();
        }
        end = time(0);
    }
}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( )
{
    conference->printConference ( );
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

// Remember to free the pointer whenever the function is called.
char* SessionOrganizer::solToStr()
{
	int pt = conference->getParallelTracks();
	int st = conference->getSessionsInTrack();
	int ps = conference->getPapersInSession();
	int n = pt*st*ps;

	char* str = (char *)malloc(totalDigitsTill(n)*4);
	for ( int i = 0; i < st; i++ )
    {
        for ( int j = 0; j < pt; j++ )
        {
            for ( int k = 0; k < ps; k++ )
            {
            	// Any number wouldn't be longer than 10 digits
            	char paperStr[10];
            	int paperId = conference->getTrack(j).getSession(i).getPaper(k);
            	sprintf(paperStr, "%d", paperId);
                strcat(str,paperStr);
            }
            if ( j != parallelTracks - 1 )
			{
				strcat(str, "| ");
			}
        }
    }
	strcat(str,"\n");
    return str;
}

vector<int> SessionOrganizer::generateRandom(int n) {
    vector<int> rndSol(n);
    for (int i = 0; i < n; i++) {
    	rndSol[i] = i;
    }
    srand(unsigned (time(0)));
    random_shuffle(rndSol.begin(), rndSol.end());
    return rndSol;
}

void SessionOrganizer::setRandom() {
	int pt = conference->getParallelTracks();
    int st = conference->getSessionsInTrack();
    int ps = conference->getPapersInSession();
    int n = pt*st*ps;
	vector<int> rndSol = generateRandom(n);

	int ijk	 = 0;
    for (int i = 0; i < st; i++)
    {
        for (int j = 0; j < pt; j++)
        {
            for (int k = 0; k < ps; k++)
            {
                conference->setPaper(j, i, k, rndSol[ijk]);
                pair<int,int> currP = make_pair(j,i);
                conference->paperInfo[rndSol[ijk]] = currP;
                ijk++;
            }
        }
    }
}

