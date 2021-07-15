#ifndef ROOT_TriFadcScin
#define ROOT_TriFadcScin

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TriFadcScin                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaNonTrackingDetector.h"
#include "Fadc250Module.h"

class THaScCalib;
class TClonesArray;

class TriFadcScin : public THaNonTrackingDetector {

public:
  TriFadcScin( const char* name, const char* description = "",
		   THaApparatus* a = NULL );
  TriFadcScin();
  virtual ~TriFadcScin();

  virtual void       Clear( Option_t* ="" );
  virtual Int_t      Decode( const THaEvData& );
  virtual EStatus    Init( const TDatime& run_time );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );

  virtual Int_t      ApplyCorrections( void );

  Int_t GetNHits() const { return fNhit; }
  const Double_t* GetTimes() const { return fTime; }
  const Double_t* GetTuncer() const { return fdTime; }
  const Double_t* GetAmplitudes() const { return fAmpl; }
  const Double_t* GetYtime() const { return fYt; }
  const Double_t* GetYampl() const { return fYa; }
  
  static const Int_t NDEST = 2;
  struct DataDest {
    Int_t*    nthit;
    Int_t*    nahit;
    Double_t*  tdc;
    Double_t*  tdc_c;
    Double_t*  adc;
    Double_t*  adc_p;
    Double_t*  adc_c;
    Double_t*  offset;
    Double_t*  ped;
    Double_t*  gain;
  } fDataDest[NDEST];     // Lookup table for decoder

  friend class THaScCalib;

  //  TriFadcScin();  // for ROOT I/O
protected:

  // Calibration
  Double_t*   fLOff;       // [fNelem] TDC offsets for left PMT
  Double_t*   fROff;       // [fNelem] TDC offsets for right PMT
  Double_t*   fLPed;       // [fNelem] ADC pedestals for left PMT
  Double_t*   fRPed;       // [fNelem] ADC pedestals for right PMT
  Double_t*   fLGain;      // [fNelem] ADC gains for left PMT
  Double_t*   fRGain;      // [fNelem] ADC gains for right PMT

  Double_t    fTdc2T;      // linear conversion between TDC and time (s/ch)
  Double_t    fCn;         // speed of light in material  (meters/second)

  Int_t       fNTWalkPar;  // number of timewalk correction parameters
  Double_t*   fTWalkPar;   // [fNTWalkPar] time walk correction parameters
  Double_t    fAdcMIP;     // nominal ADC above pedestal for MIP

  // path-lenght correction parameters
  Double_t    fXCorr;       // pl correction for X
  Double_t    fThCorr;      // pl correction for Th
  Double_t    fPhCorr;      // pl correction for Ph


  Double_t*   fTrigOff;     // [fNelem] Induced offset of trigger time from
                            // diff between trigger and retiming.
                            // Visible in coincidence data.

  Double_t    fAttenuation; // in m^-1: attenuation length of material
  Double_t    fResolution;  // average time resolution per PMT (s)

  Int_t       fNPED;        //number of samples included in FADC pedestal sum
  Int_t       fNSA;         //number of integrated samples after threshold crossing
  Int_t       fNSB;         //number of integrated samples before threshold crossing
  Int_t       fWin;         //total number of samples in FADC window
  Int_t       fTFlag;       //flag for FADC threshold on vs FADC threshold off

  // Per-event data
  Int_t       fLTNhit;     // Number of Left PMT TDC times
  Double_t*   fLT;         // [fNelem] Array of Left PMT TDC times (channels)
  Double_t*   fLT_t;       // [fNelem] Array of Left PMT times without corrections (s)
  Double_t*   fL_off;      // [fNelem] Array of Left offset corrections
  Double_t*   fLT_o;       // [fNelem] Array of Left PMT corrected TDC times without timewalk corrections (s)
  Double_t*   fLTW;        // [fNelem] Array of Left PMT TW corrections (s)
  Double_t*   fLT_c;       // [fNelem] Array of Left PMT corrected TDC times (s)
  Int_t       fRTNhit;     // Number of Right PMT TDC times
  Double_t*   fRT;         // [fNelem] Array of Right PMT TDC times (channels)
  Double_t*   fRT_t;       // [fNelem] Array of Left PMT times without corrections (s)
  Double_t*   fR_off;      // [fNelem] Array of Left offset corrections
  Double_t*   fRT_o;       // [fNelem] Array of Right PMT corrected TDC times without timewalk corrections (s)
  Double_t*   fRTW;        // [fNelem] Array of Right PMT TW corrections (s)
  Double_t*   fRT_c;       // [fNelem] Array of Right PMT corrected TDC times (s)
  Int_t       fLANhit;     // Number of Left PMT ADC amplitudes
  Double_t*   fLA;         // [fNelem] Array of Left PMT ADC amplitudes
  Double_t*   fLA_p;       // [fNelem] Array of Left PMT ADC minus ped values
  Double_t*   fLA_c;       // [fNelem] Array of Left PMT corrected ADC ampl-s
  Int_t       fRANhit;     // Number of Right PMT ADC amplitudes
  Double_t*   fRA;         // [fNelem] Array of Right PMT ADC amplitudes
  Double_t*   fRA_p;       // [fNelem] Array of Right PMT ADC minus ped values
  Double_t*   fRA_c;       // [fNelem] Array of Right PMT corrected ADC ampl-s


  Int_t      fNhit;       // Number of paddles with complete TDC hits (l&r)
  Int_t*     fHitPad;     // [fNhit] list of paddles with complete TDC hits
  Int_t*     fHitMatches; // [fNhit] list of number of tracks matching a TDC hit

  // could be done on a per-hit basis instead
  Double_t*   fTime;       // [fNelem] corrected time for the paddle (s)
  Double_t*   fdTime;      // [fNelem] uncertainty in time (s)  
  Double_t*   fT_pl;       // [fNelem] Array of paddle path-length corrections (s)
  Int_t*      fMatches;    // [fNelem] Array of number of tracks each paddel 'matches' with
  Double_t*   fAmpl;       // [fNelem] overall amplitude for the paddle
  Double_t*   fYt;         // [fNelem] y-position of hit in paddle from TDC (m)
  Double_t*   fYa;         // [fNelem] y-position of hit in paddle from ADC (m)

  //FADC
  Double_t*   fLPeak;        // [fNelem] Array of Left PMT FADC ADC peak value
  Double_t*   fLT_FADC;      // [fNelem] Array of Left PMT FADC TDC times (channels)
  Double_t*   fLT_FADC_c;    // [fNelem] Array of Left PMT corrected FADC TDC times (s)
  Int_t* floverflow;         //[fNelem] FADC overflowbit
  Int_t* flunderflow;        //[fNelem] FADC underflowbit
  Int_t* flpedq;             //[fNelem] FADC pedestal quality bit
  Int_t* flpedFADC;             //[fNelem] FADC pedestal quality bit

  Double_t*   fRPeak;        // [fNelem] Array of right PMT FADC ADC peak value
  Double_t*   fRT_FADC;      // [fNelem] Array of right PMT FADC TDC times (channels)
  Double_t*   fRT_FADC_c;    // [fNelem] Array of right PMT corrected FADC TDC times (s)
  Int_t* froverflow;         //[fNelem] FADC overflowbit
  Int_t* frunderflow;        //[fNelem] FADC underflowbit
  Int_t* frpedq;             //[fNelem] FADC pedestal quality bit
  Int_t* frpedFADC;             //[fNelem] FADC pedestal quality bit

  Int_t* fLNhits;           //[fNelem] number of hits for each Left PMT
  Int_t* fRNhits;           //[fNelem] number of hits for each Right PMT

  Decoder::Fadc250Module *fFADC;     //pointer to FADC250Module class



  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );

  enum ESide { kLeft = 0, kRight = 1 };

  virtual  Double_t TimeWalkCorrection(const Int_t& paddle,
					   const ESide side);

  ClassDef(TriFadcScin,1)   // Generic scintillator class
};

////////////////////////////////////////////////////////////////////////////////

#endif
