/*  $Name:  $
    $Date: 2009/10/05 19:32:39 $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pcap.h>
#include "tcpoe.h"

int hex_dump( const u_char *buffer , int size , int fc );
void print_hex( u_char c , int fc );
void got_packet(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet);

const char *search_str;

int main( int argc , char *argv[] ) {
  char errbuf[PCAP_ERRBUF_SIZE];
  char *dev;
  pcap_t *handle;
  pcap_dumper_t *dump;
  const u_char *packet;
  struct pcap_pkthdr header;
  struct bpf_program fp;      /* The compiled filter */
  char *filter_exp = (char*)malloc(200);          /* The filter expression */
  /*char filter_exp[] = "host 10.131.13.33";      The filter expression */
  /*bpf_u_int32 mask;		  Our netmask */
  bpf_u_int32 net;		 /* Our IP */

  if( argc < 5 ) {
    printf( "TCPS v1.0.1 $Date: 2009/10/05 19:32:39 $\n\n" );
    printf( "%s <dev> <ip> <port> <string>\n\n" , argv[0]);
    if( argc == 2 && (strcmp(argv[1],"-v")==0) )
      printf( "$Name:  $\n" );
    exit(1);
  }

  dev = argv[1];
  search_str = argv[4];

  sprintf( filter_exp , "host %s and tcp and port %s" , argv[2] , argv[3] );

  handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
    return(2);
  }

  /* Compile and apply the filter */
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return(2);
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return(2);
  }
  if ( (dump = pcap_dump_open(handle, "./out.cap" )) == NULL) {
    fprintf(stderr, "Couldn't open dump file\n" );
    return(2);
  }
  if (pcap_loop(handle, -1, got_packet, (u_char*)dump) == -1) {
    fprintf(stderr, "Couldn't set loop handle\n" );
    return(2);
  }

  /* Grab a packet */
  packet = pcap_next(handle, &header);
  /* Print its length */
  printf("Jacked a packet with length of [%d]\n", header.len);
  /* And close the session */
  pcap_close(handle);

  exit(0);
}

void got_packet(u_char *args, const struct pcap_pkthdr *header,
	    const u_char *packet) {
  /* Print its length */
  const struct sniff_ethernet *ethernet; /* The ethernet header */
  const struct sniff_ip *ip; /* The IP header */
  const struct sniff_tcp *tcp; /* The TCP header */
  const u_char *payload; /* Packet payload */

  u_int size_ip;
  u_int size_tcp;
  u_int size_data;

  ethernet = (struct sniff_ethernet*)(packet);
  ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip)*4;
  if (size_ip < 20) {
    printf("   * Invalid IP header length: %u bytes\n", size_ip);
    return;
  }

  tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = TH_OFF(tcp)*4;
  if (size_tcp < 20) {
    printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
    return;
  }

  payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
  size_data = header->len - (SIZE_ETHERNET + size_ip + size_tcp);

  if( size_data > 0 ) {
    if( strstr( (char*)payload , search_str ) != NULL ) {
      printf( "Packet length : %d\n", header->len);
      printf( "Data   length : %d\n", size_data);
      printf( "SRC_IP : %s\n" , inet_ntoa(ip->ip_src) );
      printf( "DST_IP : %s\n" , inet_ntoa(ip->ip_dst) );
      printf( "SRC_PORT : %d\n" , ntohs(tcp->th_sport) );
      printf( "DST_PORT : %d\n" , ntohs(tcp->th_dport) );
      hex_dump( payload , size_data , 1 );
      /*save_packet( packet , header->len );*/
      pcap_dump( args , header , packet );
      pcap_dump_flush( (pcap_dumper_t*)args );
    }
  memset( (u_char*)payload , 0 , size_data );
  }
}


int hex_dump( const u_char *buffer , int size , int fc ) {
  int br,count,rem,i;
  char pad[60];

  count = 0;
  rem   = size;

  do {
    if( rem == 0 ) break;
    if( rem >= 16 ) br = 16;
    else br = rem;
    printf( "%.4X:" , (count & 0xffff0000)>>16 );
    printf( "%.4X " , (count & 0x0000ffff) );
    for( i = 0 ; i < br ; i++ ) {
      print_hex( buffer[i+count] , fc );
      if( i == 7 ) printf( "- " );
    }
    printf( "  " );
    if( br < 16 ) {
      int sep = 0;
      if( br < 8 ) sep = 2;
      memset( pad , 0, sizeof(pad) );
      memset( pad , 0x20 , ((16-br)*3)+sep );
      printf( "%s" , pad );
    }
    for( i = 0 ; i < br ; i++ ) {
      if(buffer[i+count] >= 0x20 && buffer[i+count] <= 0x7e) printf( "%c" , buffer[i+count] );
        else printf( "." );
    }
    printf( "\n" );
    count += br;
    rem -= br;
  } while( br > 0 );

  return 0;
}

void print_hex( u_char c , int fc ) {
  if( fc ) {
    if( c < 0x20 ) printf( "\033[36;1m" );
    if( (c >= 0x20) && (c <=0x7e) ) printf( "\033[37;1m" );
    if( c > 0x7f ) printf( "\033[31;1m" );
  }
  printf( "%.2X " , c );
  if( fc ) printf( "\033[0m" );
}

/*
$Log: tcps.c,v $
Revision 1.1  2009/10/05 19:32:39  alberto
TCPS moved to its own dir.

Revision 1.6  2009/10/01 17:55:40  alberto
CVS Tag info added

Revision 1.5  2009/08/13 15:02:14  alberto
New version in C++ to handle fragmentation

Revision 1.4  2009/08/12 20:44:33  alberto
*** empty log message ***
*/
