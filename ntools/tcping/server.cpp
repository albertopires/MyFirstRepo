/*
    $Date: 2009/10/05 19:29:31 $
    $Name:  $
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define PORT	5000
#define VERSION "TCPING - Server v1.0.0 - 05/Oct/2009\n"

int main( int argc , char *argv[] ) {
    char buffer[2048];
    int sd, cd, br;
    int opt = 1;
    unsigned int addrlen;
    pid_t pid;
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    //struct tm *t;
    //time_t ts;

    struct sigaction sa;
    sigaction(SIGCHLD, NULL, &sa);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGCHLD, &sa, NULL);

    if( argc == 2 ) {
        if( !strcmp( argv[1] , "-v" ) ) {
            printf( VERSION );
            printf( "\n" );
            exit(0);
        }
        if( !strcmp( argv[1] , "-c" ) ) {
            printf( VERSION );
            printf( "$Name:  $\n\n" );
            exit(0);
        }
    }

    /* get an internet domain socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0) {
        perror("setsockopt ");
        exit(1);
    }

    /* complete the socket structure */
    memset(&sin, 0, sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);

    /* bind the socket to the port number */
    if (bind(sd, (struct sockaddr *) & sin, sizeof (sin)) == -1) {
        perror("bind");
        exit(1);
    }

    /* show that we are willing to listen */
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }
    /* wait for a client to talk to us */
    addrlen = sizeof (pin);

    while(1) {
        if ((cd = accept(sd, (struct sockaddr *) & pin, &addrlen)) == -1) {
            perror("accept");
            exit(1);
        }

        pid = fork();
        if( pid == 0 ) {
            printf("Source IP   : %s\n",inet_ntoa(pin.sin_addr));
            printf("Source Port : %d\n",ntohs(pin.sin_port));
	    while(1) {
                memset( buffer , 0 , sizeof(buffer) );
                br = recv( cd , buffer , 15 , MSG_WAITALL );
                if( br < 1 ) break;
                write( cd , buffer , strlen(buffer) );
            }
            close(cd);
            exit(0);
        }
        close(cd);
    }

    close(sd);

}


