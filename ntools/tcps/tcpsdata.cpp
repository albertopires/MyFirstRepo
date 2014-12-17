/*****************************************************************************/
/* Program : TCPS - String Search on a TCP Stream                            */
/* Start   : 01/October/2009                                                 */
/* Author  : Alberto Pires de Oliveira Neto - email:mrpenguin2005@gmail.com  */
/*                                                                           */
/* Last modification : 17/December/2014 by Alberto Pires de Oliveira Neto    */
/*****************************************************************************/

#include "tcpsdata.h"
void CapStream::appendData( const struct pcap_pkthdr *header , const u_char *packet ) {
	Data *d = new Data();

	memcpy( d->packet , packet , header->len );
	memcpy( (u_char*)&d->header , (u_char*)header , sizeof(struct pcap_pkthdr) );
	pack.push_back(d);
}
