/* 
 * File:   main.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>

#include "SessionOrganizer.h"

using namespace std;

/*
 * 
 */
int main ( int argc, char** argv )
{
	long start = time(0);
    // Parse the input.
    if ( argc < 1 )
    {
        cout << "./a.out <input_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    
    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer( inputfilename );

    // Organize the papers into tracks based on similarity.
    organizer->organizePapers(start);

    // cout << "organization:"<<endl;
    // organizer->printSessionOrganiser ( );
    cout << organizer->BEST_SOL;

    // Score the organization against the gold standard.
    // double score = organizer->scoreOrganization ( );
    // cout<< "score:"<<organizer->BEST_SCORE<<endl;

    return 0;
}

