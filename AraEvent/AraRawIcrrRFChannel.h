//////////////////////////////////////////////////////////////////////////////
/////  AraRawIcrrRFChannel.h        Raw RF channel class                        /////
/////                                                                    /////
/////  Description:                                                      /////
/////     A simple class for storing raw RF channel data                 /////
/////  Author: Ryan Nichol (rjn@hep.ucl.ac.uk)                           /////
//////////////////////////////////////////////////////////////////////////////

#ifndef ARARAWICRRRFCHANNEL_H
#define ARARAWICRRRFCHANNEL_H

//Includes
#include <TObject.h>
#include "araIcrrStructures.h"
#include "araIcrrDefines.h"

//!  Part of AraEvent library. The raw ICRR RF channel information. Contains the raw ADC data and LABCHIP information (RCO, hitbus...).
/*!
  The ROOT implementation of the raw ARA event data
  \ingroup rootclasses
*/
class AraRawIcrrRFChannel: public TObject
{
 public:
   AraRawIcrrRFChannel(); ///< Default constructor
   AraRawIcrrRFChannel(IcrrRFChannelFull_t *rfChan); ///< Assignment constructor
   ~AraRawIcrrRFChannel(); ///< Destructor
   void fillChannel(IcrrRFChannelFull_t *rfChan);
   
   //!  Channel Id
   /*!
     chan + 9*LAB3 (0-8 is LAB3_A, ... 18-26 are LAB3_C)
   */
   unsigned char chanId;   // chan+9*LAB3
   //!  chip id bitmask
   /*!
     0:1  LABRADOR chip
     2 RCO
     3 HITBUS wrap
     4-7 HITBUS offset
   */
   unsigned char chipIdFlag; // Bits 0,1 chipNum; Bit 3 hitBus wrap; 4-7 hitBusOff
   //!  First sample of the hitbus 
   /*!
     The last sample in the waveform is [firstHitbus-1] --unless wrapped.
     Otherwise it runs from firstHitbus+1 to lastHitbus-1 inclusive
   */
   unsigned char firstHitbus; // If wrappedHitbus=0 data runs, lastHitbus+1
   //!  Last sample of the hitbus
   /*!
     The first sample in the waveform is [lastHitbus+1] -- unless wrapped.
      Otherwise it runs from firstHitbus+1 to lastHitbus-1 inclusive
    */
   unsigned char lastHitbus; //to firstHitbus-1 inclusive
    
   UShort_t data[MAX_NUMBER_SAMPLES_LAB3]; ///< The raw sample data
   
   Int_t getLabChip() {
     return chipIdFlag&0x3;
   } ///< Returns the LABRADOR number
   Int_t getRCO() {
     return (chipIdFlag&0x4)>>2;
   } ///< Returns the RCO phase
   Int_t getFirstHitBus() {
     return firstHitbus;
   } ///< Returns the firstHitbus value for the channel
   Int_t getLastHitBus() {
     if(lastHitbus<255) return lastHitbus;
     return Int_t(lastHitbus) + Int_t((chipIdFlag)>>4);
   } ///< Returns the lastHitbus value for the channel
   Int_t getWrappedHitBus() {
    return ((chipIdFlag)&0x8)>>3;
   } ///< Return the wrapped hitbus flag for the channel. When the HITBUS is wrapped the waveform runs from firstHitbus+1 to lastHitbus-1, otherwise it runs from lastHitbus+1 to firstHitbus-1 (crossing the 259-->0 boudnary).
   
   Int_t getLatestSample(); ///<Returns the latest sample in the waveform
   Int_t getEarliestSample(); ///< Returns the earliest sample in the waveform
   
   ClassDef(AraRawIcrrRFChannel,1);
};


#endif //ARARAWICRRRFCHANNEL_H
