#include "serverheader.h"
#include <fstream>
#include <iterator>

using namespace std; 

int main() {
    //Creating a socket
    int sockAddr = serverSocket(); 

    //binding the port
    int port = 8080;
    int portBind = bindPort(port, sockAddr);  

    //Open listener
    int queueSize = 5; // Maximum number of pending connections
    int listener = openListener(sockAddr, queueSize, port);

    //Acceopting connection
    sockaddr_in address;    
    socklen_t addrlen = sizeof(address);
    int connAccept = acceptConnection(sockAddr, address);


    //what is the use of buffer?
    char buffer[1024] = {0};
    ssize_t bytesRead = readFromSocket(connAccept, buffer, sizeof(buffer));

    //parse the client request and extract the method and path.
    string requestString(buffer, bytesRead); 
    ssize_t firstSpace = requestString.find(" ");
    string method = requestString.substr(0, firstSpace);
    ssize_t secondSpace = requestString.find(" ", firstSpace+1);
    string path = requestString.substr(firstSpace+1, secondSpace-firstSpace-1);

    ifstream file("index.html");
    if(file.is_open()) { 
        cout<<"index.html file opened successfully."<<endl;
    }else {
        cout<<"Failed to open index.html file."<<endl;
    }

    string content = string(
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>()
    );

    if(method == "GET" && path == "/") { 
        cout<<"GET request received for /"<<endl;
        string response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"; 
           response += content; 
            cout<<"\nSending response back to the client for GET and index path."<<endl;
        ssize_t sentData =  send(connAccept,response.c_str(), response.size(), 0);
        if(sentData == -1) { 
            perror("Sending response failed.");
        }else{ 
            cout<<"Response sent sucessfully."<<endl;
        }
    }else if(method == "GET" && path != "/") {
        cout<<"GET request received for "<<path<<endl;
    }
    
    

    //if get request with no path, default to index.html
    //Then load the index.HTML into the memory and store its content. 
    //send it back as a resposnse. 

    close(connAccept);
    close(sockAddr); 
    return 0; 
}