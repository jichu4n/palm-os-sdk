/******************************************************************************
 * Copyright (c) 2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup Pushdemo
 */
 
/** 
 *  @brief This is the server side program for the Pushdemo sample app.
 *		   It shows how to accept a socket connection from a remote device, a 
 *		   Treo or Centro device in this case, and use this connection to push
 *		   a data packet to the mobile device.
 *		   
 *		   Note: The port number being used is 9930
 *
 *  @version 1.0	- Initial Revision (07/31/07)
 *
 *  @file TCPTest.c
 */


#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <signal.h>
#include <winsock.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFLEN 512
#define SERVER_PORT 9930 /* port number being used */


/*
 * FUNCTION: report_error
 *
 * DESCRIPTION: This routine is used to report errors
 *
 * PARAMETERS:
 *
 * s	error string
 *     
 */
 
void report_error(char *s)
{
    perror(s);
    exit(1);
}



/*
 * FUNCTION: main
 *
 * DESCRIPTION: This routine is the main function of the  C program
 *
 * PARAMETERS:
 *
 * argc		number of arguments
 *     
 * argv		pointer to array of arguments
 */

int main(void)
{
    int s, sd2, i;
    int socklen = sizeof(struct sockaddr_in);
    char buf[BUFLEN];
    int rc = 0, on = 1;
    struct sockaddr_in serveraddr;
    
    
    WSADATA  wsadata; /* This structure contains information about the Windows Sockets implementation.*/
	
    WORD     wVer;
    
    /* MAKEWORD macro creates a WORD value by concatenating the specified values. */
 	wVer = MAKEWORD(1,1);
 	
 	/* WSAStartup function initiates use of the Winsock DLL by a process */
    if (0 != (WSAStartup(wVer, &wsadata)))
    {
   		 printf ("could not open winsock\n");
    	 goto error;
    }
    
 	
 	/* Open a new socket */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("could not make a socket\n");
    	goto error;
    }
    
    
      /* The setsockopt() function is used to allow the local address to  */
      /* be reused when the server is restarted before the required wait  */
      /* time expires.                                                    */
     
    
      rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
      if (rc < 0)
      {
         printf("setsockopt(SO_REUSEADDR) failed");
         
      }

     
      /* Assign a local address to a socket */
      
      memset(&serveraddr, 0, sizeof(serveraddr));
      serveraddr.sin_family      = AF_INET;
      serveraddr.sin_port        = htons(SERVER_PORT);
      serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

      rc = bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
      if (rc < 0)
      {
         printf("bind() failed");
         
      }

      
	  /* Put the socket into passive listen mode. The socket can accept upto */
	  /* 10 incoming connections. */

      rc = listen(s, 10);
      if (rc< 0)
      {
         printf("listen() failed");
         
      }

      printf("Ready for client connect().\n");

      
      /* Accept a connection from a stream-based socket. This accept() call */
      /* will block indefinitely waiting for the incoming connection to arrive.*/
         
      sd2 = accept(s, NULL, NULL);
      if (sd2 < 0)
      {
         printf("accept() failed");
         
      }

   
    /* Send the data to the client */
       
	printf("Sending data packet to client\n");
    sprintf(buf, "This is your data.\n");

    rc = send(sd2, buf, sizeof(buf), 0);
    if (rc < 0)
    {
         
       report_error("send()");
    }


	close(s);
    return 0;
        
    error:
       printf( "Error,  exiting, errno = %d\n", errno);
    exit(1);
  return 1;
}
