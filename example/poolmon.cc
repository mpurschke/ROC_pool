
#include <iostream>
#include <pmonitor/pmonitor.h>
#include "poolmon.h"

#include <ROC_pool/ROC_pool.h>

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLatex.h>

// Example: feed one packet's datagrams into a pool and look at the
// re-assembled waveforms. The pool type follows the packet's format
// (302 or 310), so the same code works for both.
//
// h1 - waveform length (samples); with the pool, the waveforms that were
//      split across events are whole again
// h2 - persistency plot: ADC vs sample, all channels and waveforms overlaid
// h3 - for comparison, the waveform length Packet by Packet, without the
//      pool, from the same Packets; the split waveforms show up as pieces
//
// It also keeps a running tally of the pool's waveforms and samples that
// have an error ("ERROR" non-zero in any channel; for 302 these are the
// hamming error bits of the ROC header). show_errors() prints it.

int init_done = 0;

using namespace std;

static ROC_pool *pool = 0;
static int packet_id = 12001;

static unsigned long long nr_waveforms = 0;
static unsigned long long nr_error_waveforms = 0;
static unsigned long long nr_samples = 0;
static unsigned long long nr_error_samples = 0;

TH1F *h1;
TH2F *h2;
TH1F *h3;

static TCanvas *c1 = 0;

void setPacketId (const int id)
{
  packet_id = id;
}

int pinit()
{

  if (init_done) return 1;
  init_done = 1;

  h1 = new TH1F ( "h1","waveform length, from the pool;samples;waveforms", 64, -0.5, 63.5);
  h2 = new TH2F ( "h2","persistency;sample;ADC", 64, -0.5, 63.5, 256, 0, 1024);
  h3 = new TH1F ( "h3","waveform length, Packet by Packet;samples;waveforms", 64, -0.5, 63.5);

  return 0;

}

// counts the samples of waveform wf that have an error in any channel,
// and returns how many there were
static int count_error_samples(const int wf, const int ns, const int nch)
{
  int n = 0;
  for ( int s = 0; s < ns; s++)
    {
      for ( int ch = 0; ch < nch; ch++)
	{
	  if ( pool->iValue(wf, s, ch, "ERROR") )
	    {
	      n++;
	      break;
	    }
	}
    }
  return n;
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

	  int nerr = count_error_samples(wf, ns, nch);
	  nr_waveforms++;
	  nr_samples += ns;
	  nr_error_samples += nerr;
	  if ( nerr ) nr_error_waveforms++;

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

      // Packet by Packet, for comparison
      int nwf = p->iValue(0, "NR_WF");
      for ( int wf = 0; wf < nwf; wf++)
	{
	  h3->Fill(p->iValue(wf, "SAMPLESIZE"));
	}

      // and the same Packet into the pool
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

static double fraction(const unsigned long long n, const unsigned long long total)
{
  if ( !total) return 0;
  return (double) n / total;
}

void show_errors ()
{
  cout << "waveforms with errors: " << nr_error_waveforms << " of " << nr_waveforms
       << "  (" << 100. * fraction(nr_error_waveforms, nr_waveforms) << "%)" << endl;
  cout << "samples with errors:   " << nr_error_samples << " of " << nr_samples
       << "  (" << 100. * fraction(nr_error_samples, nr_samples) << "%)" << endl;
}

void poolstatus ()
{
  if ( !pool)
    {
      cout << "no pool yet" << endl;
      return;
    }
  pool->identify();
  show_errors();
}

void show ()
{
  if ( !c1)
    {
      c1 = new TCanvas("c1", "poolmon", 1500, 500);
      c1->Divide(3, 1);
    }

  c1->cd(1);
  h3->Draw();

  c1->cd(2);
  h1->Draw();

  TLatex t;
  t.SetNDC();
  t.SetTextSize(0.04);
  t.DrawLatex(0.45, 0.6, Form("waveforms with errors: %.2f%%",
			       100. * fraction(nr_error_waveforms, nr_waveforms)));
  t.DrawLatex(0.45, 0.54, Form("samples with errors: %.2f%%",
			       100. * fraction(nr_error_samples, nr_samples)));

  c1->cd(3);
  h2->Draw("colz");

  c1->Update();
}

