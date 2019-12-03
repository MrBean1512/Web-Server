# LM 5: Application Layer Programming using Sockets - Part 2  
Author: Scott Griffith  
Last Modified: 10/17/2019  
with heavy reference from Ed Walker and Kent Jones  

I expect __proper coding style__. You should know how to write and document code by now. Make sure code is properly commented and formatted. Don't be lazy. Write good code.  

You are welcome to complete this project using whatever language you like, but I am only going to be supporting C++.


### Grade Break Down
| Part | | Points |
|---|---|---|
| Part 1 | Make a Web Server | 200 pts |
| Bonus 1 | Make a personal website | Up to +40 pts |
| Bonus 2 | Implement cookies | +30 pts |
| Bonus 1 | Impliement GET/POST requests | +30 pts |
| Total | | 200 pts |



---  

## Modifying LM4 to make a Web Server  

Your last learning module was creating a client/server pair that handled the transmission of a file from one program to another. 
This time you are going to create a different server that transmits a different file to a different client.
But this time the different file is a HTML file and the different client is a web browser. Yes, that is correct. You are going to make a web server.  

Before we get started with the server, let’s talk about the client and the file. 
You can use whatever browser you would like, when you go to connect to your web server you are going to plug in the following address: http://127.0.0.1:50001 . The 50001 is the port that you should use for this project. 
The files that you are going to use are provided in the `./assets` directory of this project. This is the basic default index.html your server should respond with in the case of an empty request.  

I highly suggest you do the following, in order, when working on this:
1)	Display the HTTP requests first. This will help you debug parts later. It will also signal when a browser is connecting.
2)	Get the server to send a properly formed HTTP response with empty data.  
    * Hint: I would suggest using a stringstream object for forming the HTTP response. You can pass in “literals” and strings using the << operation.
    * Be sure to instruct the browser to close the connection using the “Connecting: close”  header field.
3)	Get the server to send properly formatted HTTP with just the basic index.html
    * Hint: I would suggest opening an ifstream object in binary mode, and pass it using a string.
4)	Get the server to handle other object requests (like the image)
    * Hint: opening the image in binary mode should work. You will not be able to ‘view’ the data but all the JPG information should pass through your system fine.
    * You are going to have to parse the HTTP request message to do this.

### Requirements:  
* Your server should respond to all GET requests in a proper manner. This means fully formed HTTP response messages. You don’t have to exhaustively map all of the HTTP response codes, but you should at least have Ok 200 and 400 Bad Request
* Your server should display the HTTP request messages in the command line. Make sure these are parsed correctly.
* You can use different webpages (.html), but your server should respond back with the default index.html file if no file/path specified. 
* Proper serving of index.html is required (i.e. your server has to parse the image request as well).

In addition to a fully functional __(and commented)__ server please provide the following in clearly labeled text files:
1)	An example HTTP request message received by your server. Annotate and describe all header fields. 
2)	An example of the HTTP response message your server sends the index.html file in. Annotate and describe all header fields

### Bonus (up to 100 pts):  

For any of the below, you can modify the index.html file to include a link to your extra credit. Make sure to document all of the below.  

__10 pts each, up to 40 pts:__ Make a personal website. 10 points per page. You are in charge of content. Make it technically interesting. If you need a hint, you could make a website outlining all of your Learning Modules. In the future, you can turn in a link to your webserver holding your LM! Super Network!  

__30 pts:__ Implement Cookies in some meaningful way. Show screenshots of different content based on different users. You can fake users using Packet Sender (LM4) or you can have other students try to connect / use different browsers.

__30 pts:__ Have some way of users sending data back to your webserver, and present that data. You can either use POST messages or parse the GET command. Either way, provide documentation of how to interact with your server. Full points if users interact directly with your page, without having to use PacketSender, or other network tools. 

