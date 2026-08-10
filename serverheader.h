#ifndef serverHeader
#define serverHeader

#include <sys/socket.h> //create an endpoint for communication.
#include <netinet/in.h>
#include <iostream>
//library for listening to requests. 
#include <unistd.h>

using namespace std; 

int serverSocket() { 
    //creating the socket : 
    int serverSocket =  socket( PF_INET, SOCK_STREAM, 0 ); 
    
    if( serverSocket == -1 ){ 
        perror("Socket creation failed."); 
        return -1; 
    }else { 
        cout<<"Socket Created"<<endl;
        return serverSocket;
    }; 
}

int bindPort(int port, int serverSocket) { 
    struct sockaddr_in address; // Need to learn more about this struct.
    address.sin_family = PF_INET; 
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == -1) { 
        perror("Binding failed.");
    }else { 
        cout<<"Binding successful."<<endl;  
    }; 
}

int openListener(int serverSocket, int queueSize, int port) { 
    int listener = listen(serverSocket, queueSize);
    if(listener == -1) { 
        perror("Listening failed."); 
    }else { 
        cout<<"Listening on port: "<<port<<endl; 
    }
}

int acceptConnection(int serverSocket, struct sockaddr_in address) { 
    socklen_t addrlen = sizeof(address);
    int connAccept = accept( serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if(connAccept   == -1) { 
        perror("Accepting connection failed.");
    }else {
        cout<<"Connection accepted."<<endl;
        return connAccept;
    } 
}

int readFromSocket(int connAccept, char* buffer, size_t bufferSize) { 
    ssize_t bytesRead = read(connAccept, buffer, bufferSize);
    if(bytesRead == -1) { 
        perror("Reading from socket failed.");
    }else {
        cout<<"Received message: "<<buffer<<endl;
        return bytesRead;
    }
}

#endif