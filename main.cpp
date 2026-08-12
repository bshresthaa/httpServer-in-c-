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

    cout<<"Waiting for incoming connections..."<<endl;
    //Acceopting connection
    sockaddr_in address;    
    socklen_t addrlen = sizeof(address);

    while(true) { 
        int connAccept = acceptConnection(sockAddr, address);
        cout << "Connection left : " << connAccept << endl;

        //what is the use of buffer?
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
            path.erase(0, 1); // Remove the leading '/' from the path
            ifstream file(path); 

            string fileContent = string(
                istreambuf_iterator<char>(file),
                istreambuf_iterator<char>()
            );

            string response = 
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "\r\n"; 
            response += fileContent; 
            cout<<"\nSending response back to the client for GET and file path."<<endl;
            ssize_t sentData =  send(connAccept,response.c_str(), response.size(), 0);
            if(sentData == -1) { 
                perror("Sending response failed.");
            }else{ 
                cout<<"Response sent sucessfully."<<endl;
            }
             
            //get the path in a queue to process. 
            // queue<string> pathQueue = getPath(path); 
            // // while(pathQueue.size() > 0) { 
            // //     cout<<pathQueue.front()<<endl;
            // //     pathQueue.pop();
            // // // }
            // // cout<<"GET request received for "<<path<<endl;

            // //Traverse a path. if path.size() > 0, then check if the dir exsits.
            // while(pathQueue.size() > 0) {
            //     if(string::npos = pathQueue.front().find(".")){ 
            //         cout<<"File: "<<pathQueue.front()<<endl;
            //         string tmpfile; 
            //         ifstream file(pathQueue.front());
            //         if(file.is_open()) { 
            //             cout<<"File opened successfully."<<endl;
            //             string content = string(
            //                 istreambuf_iterator<char>(file),
            //                 istreambuf_iterator<char>()
            //             );
            //             string response = 
            //                 "HTTP/1.1 200 OK\r\n"
            //                 "Content-Type: text/html\r\n"
            //                 "\r\n"; 
            //             response += content; 
            //             cout<<"\nSending response back to the client for GET and file path."<<endl;
            //             ssize_t sentData =  send(connAccept,response.c_str(), response.size(), 0);
            //             if(sentData == -1) { 
            //                 perror("Sending response failed.");
            //             }else{ 
            //                 cout<<"Response sent sucessfully."<<endl;
            //             }
            //             pathQueue.pop(); 
            //     }else {
            //         cout<<"cd into : "<< pathQueue.front() <<endl;
            //         string cmd = "cd " + pathQueue.front();
            //         cout<<"command line command: "<<cmd<<endl;
            //         int dirExists = system(cmd.c_str());
    
            //         if(dirExists == 0){ 
            //             cout<< "Dir exists: " << pathQueue.front() <<endl;
            //             pathQueue.pop(); 
            //         }else{
            //             perror("command failed, directory does not exist.");
            //             return -1; 
            //         }
            //     }
            // }


        }
        if(connAccept <=0 ) { 
            perror("maximum connection limit reached."); 
            close(connAccept);
        }
        buffer[0] = '\0'; // Clear the buffer for the next
        close(connAccept);
    }; 

    close(sockAddr); 
    return 0; 
}
