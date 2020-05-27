//#ifndef __TCP_PEER_H__
//#define __TCP_PEER_H__

Int32 PrvTCPOpen(Int32 in_domain, HS_HTTPLibOpaque in_aux);
void PrvTCPClose(Int32 in_desc, HS_HTTPLibOpaque in_aux);
Int32 PrvTCPIsConnected(Int32 in_desc, HS_HTTPLibOpaque in_aux);
Int32 PrvTCPConnect(Int32 in_desc, HS_HTTPLibIPAddr *in_addr, Int32 in_port, HS_HTTPLibOpaque in_aux);
Int32 PrvTCPRead(Int32 in_desc, Char *out_buf, Int32 in_len, HS_HTTPLibOpaque in_aux);
Int32 PrvTCPWrite(Int32 in_desc, Char *in_buf, Int32 in_len, HS_HTTPLibOpaque in_aux);
Int32 PrvTCPCanReadWrite(Int32 in_desc, Int32 in_rw, HS_HTTPLibOpaque in_aux);

Err PrvPeerInitialize(void);
Err PrvPeerFinalize(void);

//#endif