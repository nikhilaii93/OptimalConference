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
#include "Randomizer.h"
#include "Swapper.h"
#include <time.h>
#include <cfloat>

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
    
    double MIN_THRESHOLD = 0.005;
    double MIN_IMPROVEMENT = MIN_THRESHOLD;
    bool DO_SHOULDER = false;
    int ALLOWED_SHOULDER = 0;
    int TOTAL_SHOULDER = 0;
    int CURR_SHOULDER = 0;
    bool DO_BAD_MOVES = false;
    int ALLOWED_BAD_MOVES = 0;
    int TOTAL_BAD_MOVES = 0;
    double BAD_MOVE_THRESHOLD =  2;

    double BEST_SCORE = 0.0;
    string BEST_SOL;
    double CURR_SCORE = 0.0

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
    void organizePapers();
    
    
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
};

#endif	/* SESSIONORGANIZER_H */

