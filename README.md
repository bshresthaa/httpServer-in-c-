# C++ HTTP Server

A simple HTTP server built from scratch in C++ using Linux socket APIs.

The project is focused on understanding how HTTP communication works at a low level, including sockets, connections, request parsing, file handling, and HTTP responses.

## Features

* Creates and binds a TCP socket
* Listens for incoming connections
* Accepts client connections
* Parses basic HTTP `GET` requests
* Serves `index.html` when requesting `/`
* Serves other files from the requested path
* Sends HTTP responses directly through sockets
* Uses a simple connection queue

## How It Works

The server follows a basic HTTP request flow:

```text
Client
  ↓
TCP Connection
  ↓
accept()
  ↓
Read HTTP Request
  ↓
Parse Method + Path
  ↓
Locate Requested File
  ↓
Build HTTP Response
  ↓
send()
  ↓
Client
```

## Running the Server

Compile the project with:

```bash
g++ main.cpp -o server
```

Then start the server:

```bash
./server
```

The server listens on port `8080`.

Open in a browser:

```text
http://localhost:8080
```

## Example Requests

Requesting the root path:

```http
GET / HTTP/1.1
```

serves:

```text
index.html
```

A request such as:

```http
GET /about.html HTTP/1.1
```

attempts to serve:

```text
about.html
```

from the server's current directory.

## Project Structure

```text
httpServer/
├── main.cpp
├── serverheader.h
├── index.html
└── ...
```

## Current Limitations

This is an educational implementation and is not intended to be a production HTTP server.

Currently it has limited HTTP functionality:

* Only basic `GET` requests are handled
* Responses currently use `200 OK` even when a requested file may not exist
* Content type is currently hard-coded as `text/html`
* Connections are handled sequentially
* Request parsing is intentionally simple
* No HTTP headers such as `Content-Length` are currently generated

## Goal

The goal of this project is to build an HTTP server from the ground up and gradually add features such as better request parsing, status codes, MIME types, concurrent connections, and improved file handling.
