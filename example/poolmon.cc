
#include <iostream>
#include <pmonitor/pmonitor.h>
#include "poolmon.h"

#include <ROC_pool.h>
#include <h2gcroc3_10G_pool.h>
#include <caloroc1b_pool.h>

#include <TH1.h>
#include <TH2.h>

// Example: feed one packet's datagrams into a pool and look at the
// re-assembled waveforms. The pool type follows the packet's format
// (302 or 310), so the same code works for both.
//
// h1 - waveform length (samples); with the pool, the waveforms that were
//      split across events are whole again
// h2 - persistency plot: ADC vs sample, all channels and waveforms overlaid

int init_done = 0;

using namespace std;

static ROC_pool *pool = 0;
static int packet_id = 12001;

TH1F *h1;
TH2F *h2;

void setPacketId (const int id)
{
  packet_id = id;
}

int pinit()
{

  if (init_done) return 1;
  init_done = 1;

  h1 = new TH1F ( "h1","waveform length;samples;waveforms", 64, -0.5, 63.5);
  h2 = new TH2F ( "h2","persistency;sample;ADC", 64, -0.5, 63.5, 256, 0, 1024);

  return 0;

}

// everything on offer in the pool, until it says it needs more data
static void analyze()
{
  while ( pool->depth_ok() )
    {
      int nwf = pool->iValue(0, "NR_WF");
      int nch = pool->iValue(0, "CHANNELS");

      for ( int wf = 0; wf < nwf; wf++)
	{
	  int ns = pool->iValue(wf, "SAMPLESIZE");
	  h1->Fill(ns);

	  for ( int ch = 0; ch < nch; ch++)
	    {
	      for ( int s = 0; s < ns; s++)
		{
		  h2->Fill(s, pool->iValue(wf, s, ch));
		}
	    }
	}
      pool->next();
    }
}

int process_event (Event * e)
{

  Packet *p = e->getPacket(packet_id);
  if (p)
    {
      if ( !pool)
	{
	  pool = ROC_pool::instance(p);
	  if (!pool)
	    {
	      cout << "packet " << packet_id << " has format " << p->getHitFormat()
		   << " - no pool for that" << endl;
	      delete p;
	      return 1;
	    }

	  // switch ( p->getHitFormat() )
	  //   {
	  //   case 302:
	  //     pool = new h2gcroc3_10G_pool();
	  //     break;
	  //   case 310:
	  //     pool = new caloroc1b_pool();
	  //     break;
	  //   default:
	  //     cout << "packet " << packet_id << " has format " << p->getHitFormat()
	  // 	   << " - no pool for that" << endl;
	  //     delete p;
	  //     return 0;
	    
	}

      pool->addPacket(p);
      delete p;

      analyze();
    }
  return 0;
}

void pooldrain ()
{
  if ( !pool) return;
  pool->drain();
  analyze();
}

void poolstatus ()
{
  if ( !pool)
    {
      cout << "no pool yet" << endl;
      return;
    }
  pool->identify();
}

