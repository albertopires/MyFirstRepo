/*
    $Name:  $
    $Date: 2009/11/24 20:54:46 $
*/

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
