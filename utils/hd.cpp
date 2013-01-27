#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

using namespace std;

typedef unsigned char uchar;

int hex_dump( const char *fname , int fc );
void print_hex( uchar c , int fc );
void help( void );
void version( void );

int main( int argc , char *argv[] ) {
  int fl_color = 1;
 
  if( argc == 1 ) hex_dump( 0 , fl_color );

  if( argc > 1 ) {
    if( strcmp( argv[1] , "-nc" ) == 0 ) fl_color = 0;
    if( strcmp( argv[1] , "-h" ) == 0 ) help();
    if( strcmp( argv[1] , "--help" ) == 0 ) help();
    if( strcmp( argv[1] , "--version" ) == 0 ) version();
    if( strcmp( argv[1] , "-v" ) == 0 ) version();
  }

  if( argc == 2 ) {
    if( argv[1][0] == '-' ) hex_dump( 0 , fl_color );
    else hex_dump( argv[1] , fl_color );
  }

  if( argc == 3 ) hex_dump( argv[2] , fl_color );
}

int hex_dump( const char *fname , int fc ) {
  uchar buffer[33];
  int br,fd,count;
  char pad[60];

  count = 0;
  if( fname == 0 ) fd = 0;
  else fd = open( fname , O_RDONLY );

  if( fd < 0 ) {
    perror( "" );
    return -1;
  }

  do {
    br = read( fd , buffer , 16 );
    if( br == 0 ) break;
    printf( "%.4X:" , (count & 0xffff0000)>>16 );
    printf( "%.4X " , (count & 0x0000ffff) );
    for( int i = 0 ; i < br ; i++ ) {
      print_hex( buffer[i] , fc );
      if( i == 7 ) cout << "- ";
    }
    cout << "  ";
    if( br < 16 ) {
      int sep = 0;
      if( br < 8 ) sep = 2;
      memset( pad , 0, sizeof(pad) );
      memset( pad , 0x20 , ((16-br)*3)+sep );
      cout << pad;
    }
    for( int i = 0 ; i < br ; i++ ) {
      if(buffer[i] >= 0x20 && buffer[i] <= 0x7e) printf( "%c" , buffer[i] );
        else cout << ".";
    }
    cout << endl;
    count += br;
  } while( br > 0 );

  return 0;
}

void print_hex( uchar c , int fc ) {
  if( fc ) {
    if( c < 0x20 ) cout << "\033[36;1m";
    if( (c >= 0x20) && (c <=0x7e) ) cout << "\033[37;1m";
    if( c > 0x7f ) cout << "\033[31;1m";
  }
  printf( "%.2X " , c );
  if( fc ) cout << "\033[0m";
}

void help( void ) {
  printf( "hd <option> <file>\n\n" );
  printf( "Options:\n" );
  printf( "-nc         : Don't use ANSI color escape chars.\n" );
  printf( "-h/--help   : show this message.\n" );
  printf( "-v/-version : show version.\n\n" );
  exit(0);
}

void version( void ) {
  printf( "HD Hexdecimal Dump v1.0 - 25/Jan/2009\n" );
  printf( "By Alberto Pires de Oliveira Neto\n\n" );
  exit(0);
}

