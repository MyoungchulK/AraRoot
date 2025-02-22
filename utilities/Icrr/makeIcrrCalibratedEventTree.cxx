#include <cstdio>
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <libgen.h>     
#include <cstdlib>
 
using namespace std;

#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"

#define HACK_FOR_ROOT

#include "araIcrrStructures.h"
#include "araAtriStructures.h"
#include "RawIcrrStationEvent.h"  
#include "UsefulIcrrStationEvent.h"

void processEvent(UInt_t stationId);
void makeEventTree(char *inputName, char *outDir, UInt_t stationId);

IcrrEventBody_t theEventBody;
TFile *theFile;
TTree *eventTree;
RawIcrrStationEvent *theEvent=0;
UsefulIcrrStationEvent *theUsefulEvent=0;
char outName[FILENAME_MAX];
UInt_t realTime;
Int_t runNumber;
Int_t lastRunNumber;


int main(int argc, char **argv) {
  if(argc<4) {
    std::cout << "Usage: " << basename(argv[0]) << " <file list> <out file> <stationId (TestBed==0 Station1==1)>" << std::endl;
    return -1;
  }
  makeEventTree(argv[1],argv[2], atoi(argv[3]));
  return 0;
}
  

void makeEventTree(char *inputName, char *outFile, UInt_t stationId) {

  bool debug = false; // set to true to increase the amount of commentary output

  cout << "user specification - input file list = " << inputName << "\t" << "outFile = " << outFile << endl;
  strncpy(outName,outFile,FILENAME_MAX);
  if ( debug ) {
     cout << "                   - full outFile = " << outFile << endl;
  }
  theEvent = new RawIcrrStationEvent();
  theUsefulEvent = new UsefulIcrrStationEvent();

  //    cout << sizeof(IcrrEventBody_t) << endl;
  ifstream SillyFile(inputName);

  int numBytes=0;
  char fileName[FILENAME_MAX];
  int error=0;
  //    int eventNumber=1;

  int total_evt_count = 0;
  int file_count=0;
  while( SillyFile >> fileName ) {
    cout << "processing file: " << fileName << endl;
    file_count++;
    static int lastEventNumber=-1;    
    const char *subDir = gSystem->DirName(fileName);
    //    const char *subSubDir = gSystem->DirName(subDir);
    //    const char *eventDir = gSystem->DirName(subSubDir);
    const char *runDir = gSystem->DirName(subDir);
    const char *justRun = gSystem->BaseName(runDir);
    sscanf(justRun,"run_%d",&runNumber);
    //    cout << justRun << "\t" << runNumber <<endl;
    
    gzFile infile = gzopen(fileName, "rb");    
    numBytes=gzread(infile,&theEventBody,sizeof(IcrrEventBody_t));
    int evt_count = 1;
    total_evt_count++;
    while ( numBytes == sizeof(IcrrEventBody_t) ) {
      if ( debug                      ||
	   ( (evt_count % 100) == 1 ) ) {
         cout << "Event count: " << "for_file = " << evt_count << " - all_toll = " << total_evt_count << endl;
      }
      if(numBytes!=sizeof(IcrrEventBody_t)) {
	if(numBytes)
	  cerr << "Read problem: " <<numBytes << " of " << sizeof(IcrrEventBody_t) << endl;
	error=1;
	break;
      }
      //      cout << "Event: " << theEventBody.hd.eventNumber << endl;
      if(TMath::Abs(Double_t(theEventBody.hd.eventNumber)-lastEventNumber)>1000 && lastEventNumber>=0) {
	std::cerr << "Dodgy event\t" << theEventBody.hd.eventNumber << "\n";
      } 
      else {     
	processEvent(stationId);
	lastEventNumber=theEventBody.hd.eventNumber;
      }
      numBytes=gzread(infile,&theEventBody,sizeof(IcrrEventBody_t));
      evt_count++;
      total_evt_count++;
    } // end of while loop over events in the input file
    gzclose(infile);
    //	if(error) break;

  } // end of while loop over the ev_ files in the file list
  eventTree->AutoSave();
  //    theFile->Close();
}

void processEvent(UInt_t stationId) {
  //  cout << "processEvent:\t" << theEventBody.eventNumber << endl;
  static int doneInit=0;
  
  if(!doneInit) {
    //    char dirName[FILENAME_MAX];
    //    char fileName[FILENAME_MAX];
    //    sprintf(dirName,"%s/run%d",outDirName,runNumber);
    //    gSystem->mkdir(dirName,kTRUE);
    //    sprintf(fileName,"%s/eventFile%d.root",dirName,runNumber);
    cout << "Creating File: " << outName << endl;
    theFile = new TFile(outName,"RECREATE");
    eventTree = new TTree("eventTree","Tree of ARA Events");
    eventTree->Branch("run",&runNumber,"run/I");
    eventTree->Branch("event","RawIcrrStationEvent",&theEvent);
    eventTree->Branch("calevent","UsefulIcrrStationEvent",&theUsefulEvent);
    
    doneInit=1;
  }
  //  cout << "Here: "  << theEvent.eventNumber << endl;
  if(theEvent) delete theEvent;
  if(theUsefulEvent) delete theUsefulEvent;
  theEvent = new RawIcrrStationEvent(&theEventBody, stationId);
  theUsefulEvent = new UsefulIcrrStationEvent( theEvent , AraCalType::kLatestCalib );
  eventTree->Fill();  
  lastRunNumber=runNumber;
  //  delete theEvent;
}
