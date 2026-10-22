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

// fill level and bookkeeping, and the error tally
void poolstatus ();

// the running error tally: waveforms and samples with an error
void show_errors ();

// draw the waveform length Packet by Packet (h3) and from the pool (h1),
// with the error fractions, and the persistency plot (h2)
void show ();

#endif /* __POOLMON_H__ */
