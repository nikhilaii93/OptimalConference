/* 
 * File:   Util.h
 * Author: Kapil Thakkar
 
 */

#ifndef UTIL_H
#define	UTIL_H

/**
 * Utility Function to split string message, using give delimiter. The result is stored in result[] array.
 * 
 * @param message string to be splitted
 * @param delimter delimiter used to split string
 * @param result result will be stored in this array
 * @param expcted length of the result
 */
void splitString(string message, string delimiter, string result[], int n) {
    int i = 0, pos = 0, length = 0, temp;
    temp = message.find ( delimiter.c_str ( ), pos );
    while ( temp != -1 )
    {
        length = temp - pos;
        result[i] = message.substr ( pos, length );
        pos = temp + delimiter.size ( );
        temp = message.find ( delimiter.c_str ( ), pos );
        i++;
    }
    result[i] = message.substr ( pos );
    i++;
    if ( i != n )
    {
        cout << "The similarity matrix does not have the correct format.";
        exit ( 0 );
    }
}

int totalDigitsTill(int n) {
	// Count including 0
	int total = 1;
	int digits = 1;
	int digit_t = 9;
	while (n > digit_t) {
		total += digit_t*digits;
		n -= digit_t;
		digit_t *= 10;
		digits++;
	}
	total += n*digits;
	return total;
}

#endif	/* UTIL_H */

