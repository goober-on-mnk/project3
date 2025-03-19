#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

using namespace std;

int main() {

	Bible webBible("/home/class/csc3004/Bibles/web-complete");

	int b, c, v, r;
	LookupResult result;
	Verse verse;

	b = 1;
	c = 1;
	v = 1;
	r = 0;

	Fifo read("read");
	Fifo write("write");
	read.openread();
	write.openwrite();

	while (true) {
		
		string line = read.recv();
		Ref ref(line);

		string strbook = GetNextToken(line, ":");
		b = atoi(strbook.c_str());

		string strchap = GetNextToken(line, ":");
		c = atoi(strchap.c_str());

		string strverse = GetNextToken(line, " ");
		v = atoi(strverse.c_str());

		string strrepeat = GetNextToken(line, " ");
		r = atoi(strrepeat.c_str());

		makeMap();

		string refNum = to_string(ref.getBook()).append(":").append(to_string(ref.getChap())).append(":").append(to_string(ref.getVerse()));
		int location = webBible.indexSearch(refNum);
		if (location == -1) {
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
			else {
				result = SUCCESS;
			}
			if (result == NO_CHAPTER) {
				error = webBible.error(NO_CHAPTER);
				write.send(error);
				write.send(getName(ref.getBook()));
			}
			else if (result == NO_VERSE) {
				error = webBible.error(NO_VERSE);
				write.send(error);
				write.send(getName(ref.getBook()));
			}
			else if (result == NO_BOOK) {
				error = webBible.error(NO_BOOK);
				write.send(error);
			}
			else if (result == OTHER) {
				error = webBible.error(OTHER);
				write.send(error);
			}
			else if (result == SUCCESS){
				error = webBible.error(SUCCESS);
				write.send(error);
			}
		}
		else {
			//cout << "Result status: " << result << endl;
			verse = webBible.lookup(location);

			string reference = getName(ref.getBook()).append(" ").append(to_string(ref.getChap()).append(":").append(to_string(ref.getVerse()).append(" ")));
			string text = verse.getVerse();
			write.send(reference.append(text));

			if (r > 0) {
				for (int i = 0; i < r - 1; i++) {
					verse = webBible.nextVerse();
					ref = verse.getRef();
					string reference = getName(ref.getBook()).append(" ").append(to_string(ref.getChap()).append(":").append(to_string(ref.getVerse()).append(" ")));
					string next = verse.getVerse();
					write.send(reference.append(next));
				}
			}
		}
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