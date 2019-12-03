//////////////////////////////////////////////////////////////
// server_main.cpp											//
// Basic socket programming example of server application	//
// Last updated 11/5/2019									//
// Originally developed by Ed Walker,Kent Jones, and Scott Griffith
// Remodification by David Martin							//
//////////////////////////////////////////////////////////////
//******************************************************
//*													   *
//*   NEEDS TO HAVE ACCESS TO WINDOWS SOCKET LIBRARY!  *
//*   												   *
//*   Modify your ./.vscode/tasks.json				   *
//*   In the "build project" task include:			   *
//*   "-lwsock32" as an argument					   *
//*   												   *
//*   This should be directly after the line:		   *
//*    "\"${workspaceFolder}\"/source/*.cpp", 		   *
//*													   *
//******************************************************

#include <iostream>			//terminal output
#include <string>		  //Creation of messages
#include <fstream>		//Access to the text file
#include <sstream>
#include "../../Socket.h"	//Build winSock wrapper, shared definition by both client and server
using namespace std;

//Communication Port to connect to/from
const int MYPORT = 50001;

int done(const string message);
string get_file_size(ifstream &file_data);
void get_directory(string &msg);
string status_code(string &msg);
string content_type(string &msg);
string respond(string &msg);
Socket start(Socket &server);
string rec_req(Socket &server, Socket &conn);

//main function
//intializes socket objects, establishes connection, and sends appropraite file
int main() {
	//Initilize socket, make sure network stack is available
	if (!Socket::Init()) {
		cerr << "Fail to initialize WinSock!\n";
		return -1;
	}

	Socket server("tcp");	//create the socket object

	Socket conn = start(server);	//connect to client

	int count = 500;	//primary purpose is to limit the while loop but this is also convenient to see how many requests have been made
	//100 is an arbirary number, it's there just in case something goes wrong in the loop
	string msg;		// client sends html request 
	cout << count << ": Received message\n" << endl;
	while (count != 0){	// this loop just keeps the connection open for the client to make more requests
	//this is held open so that the client may continue to refresh and GET new pages
		count --;

		msg = conn.msg_recv();			// client sends html request 
		cout << count << ") Received message:\n\n" << msg << endl;

		conn.msg_send(respond(msg));	//create and send the message
		cout << "Message sent" << endl;
	}

	done("Press enter to exit");
	return 0;
}

//Utility Function for clean exit of program
// @input: message, display to the terminal to prompt for exit
// Closes socket and exits program
int done(const string message) {
	Socket::Cleanup();
	cout << message;
	cin.get();
	exit(0);
}

//finds the size of the passed in ifstream object
//returns the numeric size as a string
string get_file_size(ifstream &file_data){
	streampos begin,end;
	begin = file_data.tellg();			//gives the size value at the beginning
  	file_data.seekg (0, ios::end);
  	end = file_data.tellg();				//gives the size value at the end
	int file_size = (end-begin);			//file size is found by subtracting the beginning from the end
	string msg = to_string(file_size);				//interger converted to string in order to pass into the function msg_send(string)
	//cout << "File size of 'file_data' is: " << msg << " bytes.\n";
	return msg;
}

//looks at the GET in the response to determine what directory to retrieve
//passes in string pointer and alters the string
void get_directory(string &msg){
    std::istringstream temp(msg);
    std::string line;
	std::getline(temp, line);
    msg = line;
}

//returns the appropriate status code depending on whether or not the directory exists
string status_code(string &msg){
	if (msg == "!"){	//'!' is determined outside of the function as a sort of boolian message if the file fails to be initiallized
		msg = "./assets/index.html";
		return "400 Bad Request";
	}
	else{
		return "200 OK";
	}
}

//checks the string and returns the returns the content type
//returns everything after the first '.'
string content_type(string &msg){
	//this finds the file type by looking for a '.' and returns the rest of the string
	//found assistance for this at: http://www.cplusplus.com/reference/string/string/find/
	//& also at: http://www.cplusplus.com/reference/string/string/erase/

	msg.erase(0,1);	//erases the first '.'
	//ie: "./index.html" is now "/index.html"
	//did this because incrementing the position will move to the next word which breaks the program
	size_t pos = msg.find(".");
	msg = msg.substr(pos);

	//checks what kind of file there is
	//found some content types at: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types#ICO
	if (msg == ".html"){
		return "text/html";
	}
	else if (msg == ".txt"){
		return "text/plain";
	}
	else if (msg == ".ico"){
		return "image/x-icon";
	}
	else if (msg == ".jpg"){
		return "image/jpeg";
	}
	else if (msg == ".png"){
		return "image/png";
	}
	else if (msg == ".gif"){
		return "image/gif";
	}
	else{
		return "text/html";
	}
}

//forms the response message
//passes in 'msg' string object so that time is not wasted with a new 
string respond(string &msg){
	
	//antiquated code, kept it here for reference in case I need it later
	//istringstream temp(msg);
	//getline(temp, msg);
	
	//this grabs the second word (the GET) from the message
    istringstream temp2(msg);
	int n = 2;
    while(n-- > 0 && (temp2 >> msg));

	//appropriate directory is opened
	if (msg == "/")
		msg = "/index.html";

	msg = "./assets"+msg;
	cout << "Parsed GET: " << msg << endl;
	ifstream file_data (msg, ios::binary);
	if (!file_data.is_open()){			//checks whether or not the file was actually opened
		msg = "!";	//this simply gives a heads up to other programs that the file is unrecognized
	}

	//begins creating the header
	string outgoing;
	outgoing = "HTTP/1.1 " + status_code(msg) +" \n";	//appends the status code to the response message
	outgoing += "Content-Type: " + content_type(msg) + " \n";	//appends the content type to the response message
	outgoing += "Content-Length: " + get_file_size(file_data) + " \n";	//appends the centent length to the response message
	outgoing += "\n";	//paragaph break to signal end of header info

	cout << "The response header is:\n" << outgoing << endl;

	file_data.seekg(0, ios::beg);			//return to the beginning of the file so that the file isn't just read from the end
	//msg = "";								//reset the msg string which is recycled for all messages that are sent through the socket
	string line_placeholder;				//needed a placeholder to pass lines into the msg string
	while (getline(file_data,line_placeholder)){	//gets each line to read into the msg string
	  outgoing += line_placeholder + "\n";
    }
	//the msg should look something like this in the end
	//msg = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	return outgoing;
}


//establishes connection with the client
//intended to pass in 'server' Socket object constructed with 'udp' or 'tcp'
//returns 'conn' Socket object
Socket start(Socket &server){
	// Step 2: Bind socket to a port (MYPORT)	
	if (!server.sock_bind("", MYPORT)) {
		string str;
		str = "Could not bind to port " + to_string(MYPORT);
		done(str);
	}

	// Step 3: Ask my socket to "listen"
	if (!server.sock_listen(1)) 
	{
		done("Could not get socket to listen");
	}

	cout << "Server is now listening on port " << MYPORT << endl;
	
	// Step 4:	Wait to accept a connection from a client.  
	//			The variable conn is the "connected" socket.  This is the socket that is actually connected to the server.
	//			The socket variable server can be used to accept another connection if you want.
	Socket conn = server.sock_accept();
	
	cout << "A client just connected to me!\nWaiting for greeting\n";
	
	return conn;
}