#ifndef __POOLMON_H__
#define __POOLMON_H__

#include <pmonitor/pmonitor.h>
#include <Event/Event.h>
#include <Event/EventTypes.h>

int process_event (Event *e); //++CINT

// which packet to feed into the pool (default 12001)
void setPacketId (const int id);

// end of the file/stream: hand out what is left in the pool.
// Call after prun() has gone through the file.
void pooldrain ();

// fill level and bookkeeping
void poolstatus ();

#endif /* __POOLMON_H__ */
