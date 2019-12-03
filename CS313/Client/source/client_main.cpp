//////////////////////////////////////////////////////////////
// client_main.cpp											//
// Basic socket programming example of client application	//
// Last updated 10/10/2019									//
// Originally developed by Ed Walker						//
// Adapted by Kent Jones and Scott Griffith					//
//////////////////////////////////////////////////////////////
//******************************************************
//*													   *
//*   NEEDS TO HAVE ACCESS TO WINDOWS SOCKET LIBRARY!  *
//*   												   *
//*   Modify your ./.vscode/tasks.json				   *
//*   In the "build project" task include:			   *
//*   "-lwsock32" as an argument					   *
//*   												   *
//*    "\"${workspaceFolder}\"/source/*.cpp", 		   *
//*													   *
//******************************************************

#include <iostream>		  //terminal output
#include <string>		  //Creation of messages
#include "../../Socket.h" //Build winSock wrapper, shared definition by both client and server
using namespace std;

//Communication Port to connect to/from
const int MYPORT = 50001;

//Utility Function for clean exit of program
// @input: message, display to the terminal to prompt for exit
// Closes socket and exits program
int done(const string message) {
	Socket::Cleanup();
	cout << message;
	cin.get();
	exit(0);
}

void start(Socket &client){

	// Step 2: Connect to server
	// 127.0.0.1 is the IP address for the local host (this computer)
	cout << "Attempting to connect to 127.0.0.1:" << MYPORT << endl;
	if (!client.sock_connect("127.0.0.1", MYPORT)) {
		done("could not connect to server");
	}

	cout << "Connected to 127.0.0.1:" << MYPORT << endl;

	// Step 3: Successfully connected to server.  We can now send (and receive) messages.
	string msg("Hello server");
	cout << "Sending message: " << msg << endl;
	client.msg_send(msg);					// C: Hello server


	// recieve reply
	msg = client.msg_recv();				// S: Hello client!
	if (msg != "Hello client!")
	{
		done("Incorrect server greeting");
	}
	cout << "Received message: '" << msg << "'" << endl;
}


//////////////////////////////////
//LM4 function
//////////////////////////////////
void run_lm4(Socket &client){

	//request file size information from the server
	string msg("Send LMFour");
	client.msg_send(msg);					// C: Send the file

	//recieve file size information from the server
	msg = client.msg_recv();				//message recieved! Accuracy of data can't really be tested right at this step
	int file_size = stoi(msg);				//converted to interger in order to prepare and recieve the string from msg_rcvall(int). Found stoi at https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int
	cout << "File info recieved. The file is: " << file_size << " bytes." << endl;

	//request file contents from the server
	msg = "Ready for full file";
	client.msg_send(msg);
	cout << "Requesting full file." << endl;

	//recieve file contents from the server
	msg = client.msg_recvall(file_size);	//passes in the size of the file so that the client knows when to stop accepting packets
	cout << msg << "\nMessage recieved!\nClosing connection with server." << endl;	//display the contents into the console to show that the transfer was successful

	//send confirmation that the file was recieved
	cout << "Sending 'thank you'" << endl;
	msg = "Thank you.";
	client.msg_send(msg);

	//recieve closure and end program
	msg = client.msg_recv();
	if (msg != "Adios!")					//this test is arbitrary but other options could be here instead that allow the server to continue with more instructions if desired
	{
		done("Incorrect server greeting, he didn't even say goodbye");
	}
	cout << "Server repsonse says: '" << msg << "'\nEnding program." <<endl;
}


/////////////////////////////////
//LM5 function
/////////////////////////////////


void run_lm5(Socket &client){
//request file size information from the server
	string msg("Send LMFive");
	client.msg_send(msg);					// C: Send the file

	//recieve file size information from the server
	msg = client.msg_recv();				//message recieved! Accuracy of data can't really be tested right at this step
	int file_size = stoi(msg);				//converted to interger in order to prepare and recieve the string from msg_rcvall(int). Found stoi at https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int
	cout << "File info recieved. The file is: " << file_size << " bytes." << endl;

	//request file contents from the server
	msg = "Ready for full file";
	client.msg_send(msg);
	cout << "Requesting full file." << endl;

	//recieve file contents from the server
	msg = client.msg_recvall(file_size);	//passes in the size of the file so that the client knows when to stop accepting packets
	cout << msg << "\nMessage recieved!\nClosing connection with server." << endl;	//display the contents into the console to show that the transfer was successful

	//send confirmation that the file was recieved
	cout << "Sending 'thank you'" << endl;
	msg = "Thank you.";
	client.msg_send(msg);

	//recieve closure and end program
	msg = client.msg_recv();
	if (msg != "Adios!")					//this test is arbitrary but other options could be here instead that allow the server to continue with more instructions if desired
	{
		done("Incorrect server greeting, he didn't even say goodbye");
	}
	cout << "Server repsonse says: '" << msg << "'\nEnding program." <<endl;
}

/////////////////////////////////
//Main
/////////////////////////////////
// THE CLIENT: Connects to Server Application
int main() {

	//Initilize socket, make sure network stack is available
	if (!Socket::Init()) {
		cerr << "Fail to initialize WinSock!\n";
		return -1;
	}

	// Step 1: Create a TCP socket
	Socket client("tcp");

	//Establish Connection
	start(client);

	int selection = 0;
		cout << "What would you like to do?\n1: Exit Program\n2: Run LM4\n3: Run LM5\nChoose a number: ";
		cin >> selection;

		if (selection == 1){
			done("Exit Program selected. Goodbye.");
		}
		else if (selection == 2){
			cout << "Run LM4 selected." << endl;
			run_lm4(client);
		}
		else if (selection == 3){
			cout << "Run LM5 selected." << endl;
			run_lm5(client);
		}
		else if (selection == 4){
			cout << "This is a test." << endl;
		}
		else{
			done("That is not a valid choice. Ending program.");
		}

	done("Press enter to exit");
	return 0;
}

