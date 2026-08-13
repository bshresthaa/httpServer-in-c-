#include "serverheader.h"
#include <fstream>
#include <iterator>
#include <thread>

using namespace std; 

const string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"; 

void handleClient(int connAccept) { 
    sleep(10); 
        char buffer[1024] = {0};
        ssize_t bytesRead = readFromSocket(connAccept, buffer, sizeof(buffer));

        //parse the client request and extract the method and path.
        string requestString(buffer, bytesRead);  
        ssize_t firstSpace = requestString.find(" ");
        string method = requestString.substr(0, firstSpace);
        ssize_t secondSpace = requestString.find(" ", firstSpace+1);
        string path = requestString.substr(firstSpace+1, secondSpace-firstSpace-1);

        //Send response back
        if(method == "GET" && path == "/") { 
            cout<<"GET request received for /"<<endl;
                    //Open the file
            ifstream file("index.html");
            if(file.is_open()) { 
                cout<<"index.html file opened successfully."<<endl;
            }else {
                cout<<"Failed to open index.html file."<<endl;
            }

            //Read the files content
            string content = string(
                istreambuf_iterator<char>(file),
                istreambuf_iterator<char>()
            );
            string fileResponse = response; 
            fileResponse += content; 
                cout<<"\nSending response back to the client for GET and index path."<<endl;
            ssize_t sentData =  send(connAccept,fileResponse.c_str(), fileResponse.size(), 0);
            if(sentData == -1) { 
                perror("Sending response failed.");
            }else{ 
                cout<<"Response sent sucessfully."<<endl;
            }
        }else if(method == "GET" && path != "/") {
            path.erase(0, 1); // Remove the leading '/' from the path
            ifstream file(path); 

            string fileContent = string(
                istreambuf_iterator<char>(file),
                istreambuf_iterator<char>()
            );

            string fileResponse = response;
            fileResponse += fileContent; 
            cout<<"\nSending response back to the client for GET and file path."<<endl;
            ssize_t sentData =  send(connAccept,fileResponse.c_str(), fileResponse.size(), 0);
            if(sentData == -1) { 
                perror("Sending response failed.");
            }else{ 
                cout<<"Response sent sucessfully."<<endl;
            }
        }
        buffer[0] = '\0'; // Clear the buffer for the next
        close(connAccept);
}

int main() { 
    //Creating a socket
    int sockAddr = serverSocket(); 

    //binding the port
    int port = 8080;
    int portBind = bindPort(port, sockAddr);  

    //Open listener
    int queueSize = 5; // Maximum number of pending connections
    int listener = openListener(sockAddr, queueSize, port);
    cout<<"Waiting for incoming connections..."<<endl;

    int i = 0; 
    while(true) {
        //Acceopting connection
        sockaddr_in address;    
        socklen_t addrlen = sizeof(address);
        int connAccept = acceptConnection(sockAddr, address);
        if(connAccept == -1) { 
            perror("Accepting connection failed.\n");
            continue; // Skip to the next iteration of the loop
        }
        cout << "Connection accepted: " << connAccept << endl;
        thread clientThread(handleClient, connAccept); //Concurrent connection with threads. 
        cout<<"Thread created for connection: "<< i <<"th connection." <<endl;
        ++i; 
        clientThread.detach(); // Detach the thread to allow it to run independently
    }; 

    close(sockAddr); 
    return 0; 
}
