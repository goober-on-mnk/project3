/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" object, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
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
#define LOG_FILENAME "/home/class/csc3004/tmp/dergill-client.log"
#include "logfile.h"
using namespace cgicc;

int main() {
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
#ifdef logging 
	logFile.open(logFilename.c_str(), ios::out);
#endif
  cout << "Content-Type: text/plain\n\n";
  
  Cgicc cgi;  // create object used to access CGI request data

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  
  form_iterator st = cgi.getElement("search_type");      //do all of these need checked?
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");
  
  Fifo recvfifo("reply");
  Fifo sendfifo("request");

  sendfifo.openwrite();
  recvfifo.openread();

  //sendfifo.send()
  // Convert and check input data
  bool validChap = false;            //check validity of all input
  if (chapter != cgi.getElements().end()) {
	 int chapterNum = chapter->getIntegerValue();
	 if (chapterNum > 150) {
		 cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
	 }
	 else if (chapterNum <= 0) {
		 cout << "<p>The chapter must be a positive number.</p>" << endl;
	 }
	 else
		 validChap = true;
  }
  
  bool validVerse = false;
  if (verse != cgi.getElements().end()) {
	  int verseNum = verse->getIntegerValue();
	  if (verseNum > 176) {
		  cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
	  }
	  else if (verseNum <= 0) {
		  cout << "<p>The verse must be a positive number.</p>" << endl;
	  }
	  else {
		  validVerse = true;
	  }
  }

  bool validBook = false;
  if (book->getIntegerValue() < 67 && book->getIntegerValue() > 0) {
	  validBook = true;
  }
  


  int b, c, v, r;    //assigning values, idk why r is the number of verses but thats what it was in project 1
  c = chapter->getIntegerValue();
  v = verse->getIntegerValue();
  b = book->getIntegerValue();
  r = nv->getIntegerValue();

  if (r < 1) {
	  r = 1;
  }
  
  string input = to_string(b).append(":").append(to_string(c)).append(":").append(to_string(v)).append(" ").append(to_string(r));
  
  sendfifo.send(input);

  string results = recvfifo.recv();

  log("Received string: " + results);
  cout << "<p>" << results << "</p>" << endl;
  

  cout << endl;
  //recvfifo.fifoclose();
  //sendfifo.fifoclose();
  return 0;

}
