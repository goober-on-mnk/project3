#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"
#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/dergill-server.log"
#include "logfile.h"

using namespace std;


int main() {

	string reference, text, next, refNum, line;

	//declarations and stuff bla bla bla
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
#ifdef logging 
	logFile.open(logFilename.c_str(), ios::out);
#endif

	int b, c, v, r, location;
	LookupResult result;
	Verse verse;
	makeMap();

	b = 1;
	c = 1;
	v = 1;
	r = 0;

	//pipes
	Fifo recvfifo("request");
	Fifo sendfifo("reply");
	recvfifo.openread();
	sendfifo.openwrite();

	while (true) { //keep running in the background
		
		line = recvfifo.recv(); // read in line
		cout << line << endl;
		Ref ref(line);
		log("Read Input");

		//parse the line into my nice little ints for easy use in the main method (yes this is the code from Ref)
		string strbook = GetNextToken(line, ":");
		b = atoi(strbook.c_str());

		string strchap = GetNextToken(line, ":");
		c = atoi(strchap.c_str());

		string strverse = GetNextToken(line, " ");
		v = atoi(strverse.c_str());

		string strrepeat = GetNextToken(line, " ");
		r = atoi(strrepeat.c_str());

		//im sorry but this is how i decided to make the reference as a string and no im not changing it because its beautiful
		refNum = to_string(ref.getBook()).append(":").append(to_string(ref.getChap())).append(":").append(to_string(ref.getVerse()));
		location = webBible.indexSearch(refNum);
		
		if (location == -1) {      //if the reference isn't found, start error handling
			string error;
			if (b < 1 || b > 66) {
				result = NO_BOOK;
			}
			else if (c > 150 || c < 1) {
				result = NO_CHAPTER;
			}
			else if (v > 176 || v < 1) {
				result = NO_VERSE;
			}
			else {                          //this is the abomination of coding. The 'coding horror' guy from the book belongs next to this
				string badRef = refNum;     //i'm basically index searching for the ref, but when it doesnt find it, it reassigns the string 'badRef'
				for (int i = 1; i <= v; i++) {// to the same reference, but one verse or chapter smaller than before, until it finds it.
					if (webBible.indexSearch(badRef) == -1) { // I have no clue how i was able to put '- i' after those methods but it worked :)
						badRef = to_string(ref.getBook()).append(":").append(to_string(ref.getChap())).append(":").append(to_string(ref.getVerse() - i));
					}
					else {
						result = NO_VERSE;
					}
				}
				badRef = refNum;            //same thing again but for chapter
				for (int i = 1; i <= c; i++) {
					if (webBible.indexSearch(badRef) == -1) {
						badRef = to_string(ref.getBook()).append(":").append(to_string(ref.getChap() - i)).append(":").append(to_string(ref.getVerse()));
					}
					else {
						result = NO_CHAPTER;
					}
				}
			}


			//Calling error handling based on status
			if (result == NO_CHAPTER) {
				string chaperror = webBible.error(NO_CHAPTER);
				error = to_string(c).append(" does not exist in the book of ").append(getName(ref.getBook()));
				sendfifo.send(chaperror.append(error));
			}
			else if (result == NO_VERSE) {
				string verseerror = webBible.error(NO_VERSE);
				error = to_string(v).append(" does not exist in the book of ").append(getName(ref.getBook())).append(" in chapter ").append(to_string(c));
				sendfifo.send(verseerror.append(error));
			}
			else if (result == NO_BOOK) {
				string bookerror = webBible.error(NO_BOOK);
				error = to_string(b).append(" does not exist in the Bible");
				sendfifo.send(bookerror.append(error));
			}
			else if (result == OTHER) {
				error = webBible.error(OTHER);
				sendfifo.send(error);
			}
			else if (result == SUCCESS){
				error = webBible.error(SUCCESS);
				sendfifo.send(error);
			}
		}

			//if no error is detected, call lookup
			log("Past error handling");
			verse = webBible.lookup(location);

			reference = getName(ref.getBook()).append(" ").append(to_string(ref.getChap()).append(":").append(to_string(ref.getVerse()).append(" ")));
			text = verse.getVerse();
			reference.append(text);

			//cout << reference << endl;
			//write.send(reference.append(text));

			//if (r > 0) { //if r > 0, retreive that many subsequent verses
				for (int i = 1; i < r; i++) {
					//log("For loop iteration: " << i);
					verse = webBible.nextVerse();
					ref = verse.getRef();
					//cout << reference << endl;
					//log("Next verse returned");
					reference.append(getName(ref.getBook()).append(" ").append(to_string(ref.getChap()).append(":").append(to_string(ref.getVerse()).append(" "))));
					next = verse.getVerse();
					//write.send(reference.append(next));
					reference.append(next).append(" ");
					//log("Next verse appended");
					//log("r = " << r);
				}
		log("Broken out of the for loop");
		//cout << reference << endl;
		//log("Reference printed on command line");
		sendfifo.send(reference);
		log("Ref sent");
	}





	/*
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");

	Verse verse;
	int b, c, v, r;
	LookupResult result;
	//cout << "Using Bible from: ";
	//webBible.display();
	// Prompt for input: get reference to lookup
	// This is good for testing your classes quickly for the first milestone.
	//cout << "Enter 3 integers for book, chapter and verse to find: " << flush;
	//cin >> b >> c >> v;
	b = 1;
	c = 1;
	v = 1;
	if (argc != 4 && argc != 5) {
		result = OTHER;
	}
	else if (argc == 4) {
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
	}
	else if (argc == 5) {
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
		r = atoi(argv[4]);
	}

	if (b >= 67 || b <= 0) {
		result = NO_BOOK;
	}

	// Create a reference from the numbers
	Ref ref(b, c, v);

	// Use the Bible object to retrieve the verse by reference
	if (result != NO_BOOK && result != OTHER) {
		cout << "Looking up reference: ";
		makeMap();
		ref.display();
		cout << endl;
	}

	string refNum = to_string(ref.getBook()).append(":").append(to_string(ref.getChap())).append(":").append(to_string(ref.getVerse()));
	int location = webBible.indexSearch(refNum);
	if (location == -1) {
		string error;
		if (result == NO_CHAPTER) {
			error = webBible.error(NO_CHAPTER);
			cout << error << getName(ref.getBook()) << endl;
		}
		else if (result == NO_VERSE) {
			error = webBible.error(NO_VERSE);
			cout << error << getName(ref.getBook()) << " " << ref.getBook() << endl;
		}
		else if (result == NO_BOOK) {
			error = webBible.error(NO_BOOK);
			cout << error << endl;
		}
		else if (result == OTHER) {
			error = webBible.error(OTHER);
			cout << error << endl;
		}
		else {
			error = webBible.error(SUCCESS);
		}
	}
	else {
		//cout << "Result status: " << result << endl;
		verse = webBible.lookup(location);
		verse.display();
		cout << endl;
		if (argc == 5) {
			for (int i = 0; i < r - 1; i++) {
				webBible.nextVerse();
			}
		}
	}
	cout << endl;
	*/
}