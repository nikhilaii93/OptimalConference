/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"

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
}

class compare
{
public:
    compare(){};
    bool operator() (const pair<pair<int,int>,double>& s1, const pair<<pair<int,int>,double>& s2)
    {
        return (s1.second < s2.second);
    }
};

vector<pair<pair<int,int>,double> > getSwaps()
{
    int pt = conference->getParallelTracks();
    int st = conference->getSessionsInTrack();
    int ps = conference->getPapersInSession();
    int n = pt*st*ps;
    vector<pair<pair<int,int>,double> > possSwaps;
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            double diff = getPaperSwapDiff(i,j);
            if (diff >= MIN_IMPROVEMENT
                || (DO_SHOULDER && diff >= 0)
                || DO_BAD_MOVES)
            {
                pair<int,int> currP = make_pair(i,j);
                pair<pair<int,int>,double> currS = make_pair(currP,diff);
                possSwaps.push_back(currS);
            }
        }
    }
    return possSwaps;
}

pair<pair<int,int>,double> getValidSwap(vector<pair<pair<int,int>,double> > possSwaps)
{   
    pair<int,int> validSwap = NULL;
    for (int i = possSwaps.size()-1; i >= 0 ; i--)
    {
        pair<<pair<int,int>,double> possSwap = possSwaps[i];
            if (TABU_LIST_SIZE > 0)
            {
                swapPapers(this, possSwap.first.first, possSwap.first.second);
                string possStr = solToStr(this);
                swapPapers(this, possSwap.first.first, possSwap.first.second);
                if (checkTabu(possStr)
                {
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

bool doSomething()
{   
    srand (time(NULL));
    DO_BAD_MOVES = (rand() % 10 + 1 > BAD_MOVE_THRESHOLD);
    DO_SHOULDER = (CURR_SHOULDER < ALLOWED_SHOULDER);

    vector<pair<pair<int,int>,double> > possSwaps = getSwaps();

    boolean DO_RANDOM_SWAPS = false;
    if (rand() % 10 + 1 > BAD_MOVE_THRESHOLD)
    {
        random_shuffle(possSwaps);
        DO_RANDOM_SWAPS = true;
    }
    else
    {
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

    pair<int,int> selSwap = getValidSwap(possSwaps);

    if (selSwap != NULL)
    {
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

        swapPapers(this, selSwap.first.first, selSwap.first.second);

        double score = CURR_SCORE + selSwap.second;

        int session1 = conference->paperInfo[selSwap.first.first].second;
        int session2 = conference->paperInfo[selSwap.first.second].second;
        
        enqueueTabu(this);
        CURR_SCORE = score;
        cout << "CURR_SCORE: " << CURR_SCORE << " BEST_SCORE: " << BEST_SCORE << endl;
        if (CURR_SCORE > BEST_SCORE) {
            BEST_SCORE = CURR_SCORE;
            BEST_SOL = solToStr(this);
        }
        return true;
    } else {
        return false;
    }
}

void SessionOrganizer::organizePapers()
{   
    long start = time(0);
    while(processingTimeInMinutes*60-10 > end-start) {
        if (!doSomething()) {
            setRandom();
        }
        long end = time(0);
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
