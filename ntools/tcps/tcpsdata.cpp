/*
    $Name:  $
    $Date: 2009/10/05 19:32:39 $
*/

#include "tcpsdata.h"
void CapStream::appendData( const struct pcap_pkthdr *header , const u_char *packet ) {
  Data *d = new Data();

  memcpy( d->packet , packet , header->len );
  memcpy( (u_char*)&d->header , (u_char*)header , sizeof(struct pcap_pkthdr) );
  pack.push_back(d);
}
