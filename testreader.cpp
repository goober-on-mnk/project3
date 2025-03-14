#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv) {
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
	
}