/*****************************************************************************/
/* Program : TCPS - String Search on a TCP Stream                            */
/* Start   : 01/October/2009                                                 */
/* Author  : Alberto Pires de Oliveira Neto - email:mrpenguin2005@gmail.com  */
/*                                                                           */
/* Last modification : 19/June/2012 by Alberto Pires de Oliveira Neto        */
/*****************************************************************************/

#ifndef _TCPSDATA_H
#define _TCPSDATA_H

#include <pcap.h>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

struct Data {
	struct pcap_pkthdr header;
	u_char packet[65535];
};

class CapStream {
	public:
	vector<struct Data*> pack;
	string text;

	void appendData( const struct pcap_pkthdr *header , const u_char *packet );
};

#endif
