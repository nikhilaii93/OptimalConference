/* 
 * File:   SessionOrganizer.h
 * Author: Kapil Thakkar
 *
 */

#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

#include "Conference.h"
#include "Track.h"
#include "Session.h"
#include "TabuList.h"
#include "Swapper.h"
#include <time.h>
#include <cfloat>
#include <cmath>

using namespace std;


/**
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 * 
 * @author Kapil Thakkar
 *
 */
class SessionOrganizer {
private:
    double ** distanceMatrix;

    int parallelTracks ;
    int papersInSession ;
    int sessionsInTrack ;

    Conference *conference;

    double processingTimeInMinutes ;
    double tradeoffCoefficient ; // the tradeoff coefficient


public:
    SessionOrganizer();
    SessionOrganizer(string filename);
    
    int TABU_SIZE;
    double MIN_THRESHOLD;
    double MIN_IMPROVEMENT;
    bool DO_SHOULDER;
    int ALLOWED_SHOULDER;
    int TOTAL_SHOULDER;
    int CURR_SHOULDER;
    bool DO_BAD_MOVES;
    int ALLOWED_BAD_MOVES;
    int TOTAL_BAD_MOVES;
    double BAD_MOVE_THRESHOLD;

    double BEST_SCORE;
    char* BEST_SOL;
    double CURR_SCORE;

    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     * @return the similarity matrix.
     */
    void readInInputFile(string filename);
    
    
    
    /**
     * Organize the papers according to some algorithm.
     */
    void organizePapers(long start);
    
    
    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();
    
    
    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();
    
    
    void printSessionOrganiser();

    vector<pair<pair<int,int>,double> > getSwaps();

    pair<pair<int,int>,double> getValidSwap(vector<pair<pair<int,int>,double> > possSwaps);

    bool doSomething();

    char* solToStr();

    vector<int> generateRandom(int n);

    void setRandom();
};

#endif	/* SESSIONORGANIZER_H */

