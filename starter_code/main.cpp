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
    organizer->organizePapers ( );

    cout << "organization:"<<endl;
    organizer->printSessionOrganiser ( );

    // Score the organization against the gold standard.
    double score = organizer->scoreOrganization ( );
    cout<< "score:"<<score<<endl;

    return 0;
}

