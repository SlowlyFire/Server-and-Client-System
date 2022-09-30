# Server-and-Client-System
**This project is basically an implementation of Server and Client System, based on signals communication and transferring data through files**.  
 
**in this project, we implement a server, which represents a simple calculator**, that supports 4 basic operations: add, substraction, multiply and division.  
The server gets from the client 2 numbers and one of the requested operations.  
The server calculates, and returns to the client the result.  

## How Does it Works?  

- The server is running at the **background**, so it can get any client request at any time.  
- Client's request signals to the server, so the server can manage the request.  
- By using "ps" command, we get the server PID. we should keep its PID, in order to communicate between clients and the server along the program (works instead of using URL). 
- **Several client programs must be run at the same time**.  
- **Each client request will get 4 arguments**, as the following:  
*ex4_client.out P1 P2 P3 P4*    
While Pi means:  
  - **P1** is the server PID (stays constant for the whole   program run).  
  - **P2** is the first number.  
  - **P3** is the operator we use: 1 for add, 2 for substraction, 3 for multiply and 4 for division.  
  - **P4** is the second number.  

**For example:**    
  ex4_client.out 1234 70 2 30  
We can see that the client wants to connect to a server with PID 1234, and wish to calculate 70-30.  
The server should return 40 as a result, back to the client.  

- Once the server gets a signal from a client, the server creates a child proccess, so its job is to manage the client request, so the father proccess can wait for another signals of another clients.
- The client is waiting for a result from the server, but the client is not patient, so **if client is waiting over 30 sec to a result**, then we print "Client closed because no response was received from the server for 30 seconds". 
- **Otherwise**, the client gets the result from the server in time, and **print to the screen the result.**   Then the client closes all his files.  
- We should also notice that **if the server doesn't get any request from clients for over 60 sec**, the server stops running and prints: "The server was closed because no service rquest was received for the last 60 seconds".  


