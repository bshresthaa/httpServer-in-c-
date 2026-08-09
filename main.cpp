#include <iostream>
//library for listening to requests. 
#include <sys/socket.h> //create an endpoint for communication.
#include <netinet/in.h>
#include <unistd.h>

using namespace std; 

int main() {
    //creating the socket : 
    int serverSocket =  socket( PF_INET, SOCK_STREAM, 0 ); 
    
    if( serverSocket == -1 ){ 
        perror("Socket creation failed."); 
        return -1; 
    }else { 
        cout<<"Socket Created"<<endl;
    }; 

    //binding the port
    int port = 8080; 
    struct sockaddr_in address; // Need to learn more about this struct.
        address.sin_family = PF_INET; 
        address.sin_port = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == -1) { 
        perror("Binding failed.");
    }else { 
        cout<<"Binding successful."<<endl;
    }; 

    //Open listener
    int listener = listen(serverSocket, 5);
    if(listener == -1) { 
        perror("Listening failed."); 
    }else { 
        cout<<"Listening on port: "<<port<<endl; 
    }

    //Acceopting connection
    socklen_t addrlen = sizeof(address);
    int connAccept = accept( serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if(connAccept   == -1) { 
        perror("Accepting connection failed.");
    }else {
        cout<<"Connection accepted."<<endl;
        cout<<connAccept<<endl;
    } 

    //what is the use of buffer?
    char buffer[1024] = {0};
    ssize_t bytesRead = read(connAccept, buffer, sizeof(buffer));
    if(bytesRead == -1) { 
        perror("Reading from socket failed.");
    }else {
        cout<<"Received message: "<<buffer<<endl;
    }

    string requestString(buffer, bytesRead); 
    ssize_t firstSpace = requestString.find(" ");
    string method = requestString.substr(0, firstSpace);

    ssize_t secondSpace = requestString.find(" ", firstSpace+1);
    string path = requestString.substr(firstSpace+1, secondSpace-firstSpace-1);

    cout<< "Method: "<<method<<endl;
    cout<< "Path: "<<path<<endl;

    if(method == "GET" && path == "/") { 
        cout<<"GET request received for /"<<endl;
    }else if(method == "GET" && path != "/") {
        cout<<"GET request received for "<<path<<endl;
    }
    


    //parse the message and server the content back. 
    //GET / HTTP/1.1
    // Host: localhost:8080
    // Connection: keep-alive
    // sec-ch-ua: "Chromium";v="146", "Not-A.Brand";v="24", "Google Chrome";v="146"
    // sec-ch-ua-mobile: ?0
    // sec-ch-ua-platform: "macOS"
    // Upgrade-Insecure-Requests: 1
    // User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/146.0.0.0 Safari/537.36
    // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
    // Sec-Fetch-Site: none
    // Sec-Fetch-Mode: navigate
    // Sec-Fetch-User: ?1
    // Sec-Fetch-Dest: document
    // Accept-Encoding: gzip, deflate, br, zstd
    // Accept-Language: en-US,en;q=0.9
    // Cookie: PHPSESSID=4cfffe3d8837509a814aafbac6d3e8fb 

    // \r\n line start one line down, this is how the HTML response is structured.
    
    cout<<"\nSending response back to the client."<<endl;
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
    ssize_t sentData =  send(connAccept,response, sizeof(response), 0);
    if(sentData == -1) { 
        perror("Sending response failed.");
    }else{ 
        cout<<"Response sent sucessfully."<<endl;
    }

    //read the request


    //if get request with no path, default to index.html
    //Then load the index.HTML into the memory and store its content. 
    //send it back as a resposnse. 

    close(connAccept);
    close(serverSocket); 
    return 0; 
}