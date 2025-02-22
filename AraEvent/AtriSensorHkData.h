//////////////////////////////////////////////////////////////////////////////
/////  AtriSensorHkData.h        Sensor HK data class                  /////
/////                                                                    /////
/////  Description:                                                      /////
/////     A simple class for storing Sensor related housekeeping info    /////
/////  Author: Ryan Nichol (rjn@hep.ucl.ac.uk)                           /////
//////////////////////////////////////////////////////////////////////////////

#ifndef ATRISENSORHKDATA_H
#define ATRISENSORHKDATA_H

//Includes
#include <TObject.h>
#include "araSoft.h"
#include "araAtriStructures.h"
#include "RawAraGenericHeader.h"

//!  Part of AraEvent library. Class for storing ATRI sensor housekeeping data.
/*!
  The ROOT implementation of the HK data
  \ingroup rootclasses
*/
class AtriSensorHkData: public RawAraGenericHeader, public TObject
{
 public:
   AtriSensorHkData(); ///< Default constructor
   AtriSensorHkData(AraSensorHk_t *theHk); ///< Assignment constructor
   ~AtriSensorHkData(); ///< Destructor

   ULong64_t unixTime; ///< Time in seconds (64-bits for future proofing)
   UInt_t unixTimeUs; ///< Time in microseconds (32-bits)

   
   UChar_t atriVoltage; ///< ATRI Voltage (multiply by 0.065 to get Volts)
   UChar_t atriCurrent; ///< ATRI Current (multiply by (0.151)/2. to get Amps)
   UShort_t ddaTemp[DDA_PER_ATRI]; ///< DDA Temperature conversion??
   UShort_t tdaTemp[TDA_PER_ATRI]; ///< TDA Temperature conversion??
   UInt_t ddaVoltageCurrent[DDA_PER_ATRI]; ///< 3 bytes only will work out better packing when I know what the numbers mean
   UInt_t tdaVoltageCurrent[DDA_PER_ATRI]; ///< 3 bytes only will work out better packing when I know what the numbers mean

   Double_t getDdaVoltage(Int_t dda); ///< Unpack the dda voltage (Volts)
   Double_t getDdaCurrent(Int_t dda); ///< Unpack the dda current (Amps)
   Double_t getTdaVoltage(Int_t tda); ///< Unpack the tda voltage (Volts)
   Double_t getTdaCurrent(Int_t tda); ///< Unpack the tda current (Amps)
   
   Double_t getDdaTemp(Int_t dda); ///<Unpack the dda temperature (C)
   Double_t getTdaTemp(Int_t tda); ///<Unpack the tda temperature (C)
   
   Double_t getAtriVoltage(); ///< PSA Voltage should be ADC value (8 bits, 0-256) times 0.0605 (to get it in volts).
   Double_t getAtriCurrent(); ///< Current should be ADC value times 0.0755 (to get it in amps).


   ClassDef(AtriSensorHkData,2);
};


#endif //ATRISENSORHKDATA
