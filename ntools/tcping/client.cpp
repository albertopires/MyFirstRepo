/*
    $Date: 2009/10/06 22:07:43 $
    $Name:  $
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#define VERSION "TCPING - Client v1.0.1 - 06/Oct/2009\n"

void timeout( int sig );
void log_timeout( const char *logMsg );

int fl_timeout;
int port;

int main(int argc, char **argv) {
    char hostname[100];
    char buffer[2048];
    char msg[128];
    int sd;
    time_t ts;
    struct tm *t;
    struct sockaddr_in pin;
    struct hostent *hp;

    if( argc == 2 ) {
        if( !strcmp( argv[1] , "-v" ) ) {
            printf( VERSION );
            printf( "\n" );
        }
        if( !strcmp( argv[1] , "-c" ) ) {
            printf( VERSION );
            printf( "$Name:  $\n\n" );
        }
    }

    if (argc < 3) {
        printf("client <host> <port>\n");
        exit(1);
    }

    strcpy(hostname, argv[1]);
    port = atoi(argv[2]);

    /* go find out about the desired host machine */
    if ((hp = gethostbyname(hostname)) == 0) {
        perror("gethostbyname");
        exit(1);
    }

    /* fill in the socket structure with host information */
    memset(&pin, 0, sizeof (pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
    pin.sin_port = htons(port);

    /* grab an Internet domain socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* connect to PORT on HOST */
    if (connect(sd, (struct sockaddr *) & pin, sizeof (pin)) == -1) {
        perror("connect");
        exit(1);
    }

    signal(SIGALRM, timeout);
    while (1) {
        memset(buffer, 0, sizeof (buffer));
        strcpy(buffer, "0123456789ABCDE");
        write(sd, buffer, strlen(buffer));
        ts = time(NULL);
        t = localtime(&ts);
        printf("%.2d:%.2d:%.2d - snd\n", t->tm_hour, t->tm_min, t->tm_sec);

        memset(buffer, 0, sizeof (buffer));
        alarm(3);
        fl_timeout = 0;
        recv(sd, buffer, 15 , MSG_WAITALL);
        ts = time(NULL);
        t = localtime(&ts);
        if( fl_timeout ) {
          memset( msg , 0 , sizeof(msg) );
          sprintf(msg,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d - Return from Rcv TIMEOUT\n",
              t->tm_year+1900, t->tm_mon+1, t->tm_mday,
              t->tm_hour, t->tm_min, t->tm_sec );
          log_timeout(msg);
        }
        printf("%.2d:%.2d:%.2d - ", t->tm_hour, t->tm_min, t->tm_sec);
        printf("Buffer : <%s>\n", buffer);
        sleep(2);
    }

    close(sd);
    exit(0);
}

void timeout( int sig ) {
    struct tm *t;
    time_t ts;
    char msg[128];
    /*
     * Ignore the signal for the duration of this
     * routine.
     */
    signal(sig, SIG_IGN);
  
    /* 
     * We would perform any timeout-related 
     * functions here.
     */
    memset( msg , 0 , sizeof(msg) );
    ts = time(NULL);
    t = localtime(&ts);
    sprintf(msg,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d - Client Rcv TIMEOUT\n",
        t->tm_year+1900, t->tm_mon+1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec);
    log_timeout(msg);
    fl_timeout = 1;

    /*
     * Restore the action of the alarm signal
     */
    signal(SIGALRM, timeout);
}

void log_timeout( const char *logMsg ) {
    int fd;
  
    char fname[64];
    memset( fname , 0 , sizeof(fname) );
    sprintf( fname , "./timeout_%d.log" , port ); 

    fd = open( fname , O_WRONLY | O_APPEND | O_CREAT , 0000666 );
    if( fd == -1 ) {
      perror( "Client - log file error " );
      exit(0);
    }

    write( fd , logMsg , strlen(logMsg) );

    close(fd);
}
