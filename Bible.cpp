// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <map>
#include <utility>
#include <set>
#include <sstream>

using namespace std;

ifstream infile;

Bible::Bible() { // Default constructor
	string in = "/home/class/csc3004/Bibles/web-complete";
	buildTextIndex(in);
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	in = s; 
	buildTextIndex(in);
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(int location) {

	string line;

	infile.clear();
	infile.seekg(location);
	
	getline(infile, line);
	Verse final(line);
	return final;
	/*
	if (status == NO_BOOK || status == OTHER) {
		final = Verse();
		return final;
	}
	status = OTHER;
	instream.open(infile, ifstream::in);
	string line;
	while (status == OTHER) {
		getline(instream, line);
		Ref r(line);
		if (r == ref) {
			status = SUCCESS;
		}
		if (r < ref) {
			status = OTHER;
		}
		if (r > ref) {
			if (r.getChap() > ref.getChap()) {
				status = NO_VERSE;
			}
			else if (r.getBook() > ref.getBook()) {      //1:50:30 2:1:1
				status = NO_CHAPTER;                //1:51:20 2:1:1
			}
			
		}
	}
	if (status == SUCCESS) {
		final = Verse(line);
	}
	else {
		final = Verse();
	}
	*/
	return final;
}
// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse() {
	string line;
	//infile.clear();
	getline(infile, line);
	Verse next(line);
	//next.display();
	return next;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	if (status == NO_CHAPTER) {
		return "Error: Chapter ";
	}
	else if (status == NO_VERSE) {
		return "Error: Verse ";
	}
	else if (status == NO_BOOK) {
		return "Error: Book ";
	}
	else if (status == OTHER) {
		return "Error: Invalid number of entries detected";
	}
	else {
		return "Reference not found";
	}
}
void Bible::display() {
	cout << "Bible file: " << in << endl;
}


// to be accessed and updated only by functions below

/*****************************************************************************/
/*      string getNextWord(string&)                                          */
/*          given a string, return the next word,                            */
/*          and remove this word from the string (a destructive function)    */
/*****************************************************************************/
string Bible::getNextWord(string& line) {
	string word = "";
	int pos;

	if (line.length() > 1)
	{
		pos = line.find_first_of(".,; :?!""''()");
		/* get next word */
		if (pos != 0) {
			word = line.substr(0, pos);
		}
		/* trim the line */
		if (pos == string::npos) {
			pos = line.length() - 1;
		}
		line = line.substr(pos + 1, 2000);
	}
	return word;
}


/*****************************************************************************/
/*      buildTextIndex - reads text file and constructs inverted index       */
/*****************************************************************************/
int Bible::buildTextIndex(string filename) {
	//cout << "Building Index..." << endl;
	int position;         /* location of line in the file */
	string ref, lineline;
	int wordcount = 0, pos;
	infile.seekg(0);

	//cout << "Opening file" << endl;
	/* open text file */
	infile.open(in);
	if (!infile) {
		cerr << "Error - can't open input file: " << filename << endl;
		return 0;  /* false, indicates error */
	}
	position = infile.tellg();
	/* process text file */
	//cout << "Getting text from Bible file" << endl;
	Ref re;
	while (!infile.fail()) {
		/* get the next line */
		position = infile.tellg();
		getline(infile, lineline);
		if (position == -1) {

		}
		else if (lineline == "") {

		}
		else {
			//cout << "line = " << lineline << endl;
			//cout << "position = " << position << endl;
			//ref = lineline.find_first_of(".,; ?!""''()");
			string delimiter = " ";
			string ref = lineline.substr(0, lineline.find(delimiter));
			//getLine(ss, ref, ' ');
			//cout << "ref = " << ref << endl;
			re = Ref(ref);
			refs.insert({ re, position });
		}

	} // end while loop for lines of file
	return 1;  /* true, indicates success */
}

/*****************************************************************************/
/*  indexSearch(string)                                                      */
/*      returns a list of offsets for all lines                              */
/*      containing a word that matches the input string                      */
/*****************************************************************************/
int Bible::indexSearch(string reference) {
	map<Ref, int>::iterator it;  // iterator for find

	/* First use find, so as to NOT create a new entry in refs */
	it = refs.find(reference);
	if (it == refs.end()) {
		return(-1);
	}
	else {
		return (refs[reference]);
	}
}
