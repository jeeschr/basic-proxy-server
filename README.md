proxy.c - A Simple Sequential Web proxy                                                                                                
                    
Opens a socket and listens for connection. It enters a loop where if an HTTP request is made through browser it opens a connection to the host and it prints the request on the terminal. The request is forwarded to the server, response received, and printed to the browser. Works on simple sites like www.example.com.    

To use:

Configure browser to go through proxy on localhost.  