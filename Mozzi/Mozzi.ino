//////////////////////////////////Sinewave/////////////////////////////////////
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
Oscil <COS8192_NUM_CELLS, AUDIO_RATE> aSin(COS8192_DATA);
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Line_Gliss///////////////////////////////////
#include <MozziGuts.h>
#include <Line.h>
#include <Oscil.h>
#include <tables/triangle_warm8192_int8.h>
#include <mozzi_midi.h>
Oscil <TRIANGLE_WARM8192_NUM_CELLS, AUDIO_RATE> aTriangle(TRIANGLE_WARM8192_DATA);
Line <long> aGliss;
byte lo_note = 24;
byte hi_note = 36;
long audio_steps_per_gliss = AUDIO_RATE / 4;
long control_steps_per_gliss = CONTROL_RATE / 4;
int counter = 0;
byte gliss_offset = 0;
byte gliss_offset_step = 2;
byte  gliss_offset_max = 36;
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////PWM_Phasing//////////////////////////////////
#include <MozziGuts.h>
#include <Phasor.h>
Phasor <AUDIO_RATE> aPhasor1;
Phasor <AUDIO_RATE> aPhasor2;
float freq = 55.f;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////AMsynth/////////////////////////////////////
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/COS8192_int8.h>
#include <mozzi_fixmath.h>
#include <EventDelay.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCarrier(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModulator(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModDepth(COS8192_DATA);
EventDelay  kNoteChangeDelay;
Q8n8 ratio;
Q24n8 carrier_freq;
Q24n8 mod_freq;
Q8n0 octave_start_note = 42;
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////ReverbTank////////////////////////////////
#include <MozziGuts.h>
#include <ReverbTank.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
#include <tables/envelop2048_uint8.h>
ReverbTank reverb;
Oscil <COS8192_NUM_CELLS, AUDIO_RATE> bCarrier(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, AUDIO_RATE> bModulator(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, AUDIO_RATE> aModWidth(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, CONTROL_RATE> kModFreq1(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, CONTROL_RATE> kModFreq2(COS8192_DATA);
Oscil <ENVELOP2048_NUM_CELLS, AUDIO_RATE> aEnvelop(ENVELOP2048_DATA);
///////////////////////////////////////////////////////////////////////////////

/*
Above is the inclusion of files, tables, and libraries from the MOZZI library that each sound requires.
These are referrenced later in order to create synthesized effects.
Below is how each sound manipulates the included libraries and tables in order to create the synthesized sounds.
In order for this to work, the main MOZZI library must be downloaded from http://sensorium.github.io/Mozzi/
*/

#define CONTROL_RATE 64

void setup(){
  startMozzi(CONTROL_RATE);    //All//
  randSeed();                  //Multiple//
  aSin.setFreq(440);           //Sinewave//
  aPhasor1.setFreq(freq);      //PWM_Phasing//
  aPhasor2.setFreq(freq+0.2f); //PWM_Phasing//
  ratio = float_to_Q8n8(3.0f); //AMsynth//
  kNoteChangeDelay.set(200);   //AMsynth//
  aModDepth.setFreq(13.f);     //AMsynth//
  bCarrier.setFreq(55);        //ReverbTank//
  kModFreq1.setFreq(3.98f);    //ReverbTank//
  kModFreq2.setFreq(3.31757f); //ReverbTank//
  aModWidth.setFreq(2.52434f); //ReverbTank//
  aEnvelop.setFreq(9.0f);      //ReverbTank//
}

void updateControl(){
  if (--counter <= 0){                                                      //Line_Gliss//
    gliss_offset += gliss_offset_step;                                      //Line_Gliss//
    if(gliss_offset >= gliss_offset_max) gliss_offset=0;                    //Line_Gliss//
    int start_freq = mtof(lo_note+gliss_offset);                            //Line_Gliss//
    int end_freq = mtof(hi_note+gliss_offset);                              //Line_Gliss//
    long gliss_start = aTriangle.phaseIncFromFreq(start_freq);              //Line_Gliss//
    long gliss_end = aTriangle.phaseIncFromFreq(end_freq);                  //Line_Gliss//
    aGliss.set(gliss_start, gliss_end, audio_steps_per_gliss);              //Line_Gliss//
    counter = control_steps_per_gliss;                                      //Line_Gliss//
  }                                                                         //Line_Gliss//
  static Q16n16 last_note = octave_start_note;                              //AMsynth//
  if(kNoteChangeDelay.ready()){                                             //AMsynth//
    if(rand((byte)5)==0){                                                   //AMsynth//
      last_note = 36+(rand((byte)6)*12);                                    //AMsynth//
    }                                                                       //AMsynth//
    if(rand((byte)13)==0){                                                  //AMsynth//
      last_note += 1-rand((byte)3);                                         //AMsynth//
    }                                                                       //AMsynth//
    if(rand((byte)5)==0){                                                   //AMsynth//
      ratio = ((Q8n8) 1+ rand((byte)5)) <<8;                                //AMsynth//
    }                                                                       //AMsynth//
    if(rand((byte)5)==0){                                                   //AMsynth//
      ratio += rand((byte)255);                                             //AMsynth//
    }                                                                       //AMsynth//
    last_note += 3 * (1-rand((byte)3));                                     //AMsynth//
    Q16n16 midi_note = Q8n0_to_Q16n16(last_note);                           //AMsynth//
    carrier_freq = Q16n16_to_Q24n8(Q16n16_mtof(midi_note));                 //AMsynth//
    mod_freq = (carrier_freq * ratio)>>8;                                   //AMsynth//
    aCarrier.setFreq_Q24n8(carrier_freq);                                   //AMsynth//
    aModulator.setFreq_Q24n8(mod_freq);                                     //AMsynth//
    kNoteChangeDelay.start();                                               //AMsynth//
  }                                                                         //AMsynth//
  bModulator.setFreq(277.0f + 0.4313f*kModFreq1.next() + kModFreq2.next()); //ReverbTank//
}

int updateAudio(){
  
  //The while statements determine what sound gets played when pressing a button.
  
  while ((digitalRead(2) == LOW) ) {                                            //Sinewave//
    return aSin.next();                                                         //Sinewave//
  }                                                                             //Sinewave//
  while (digitalRead(4) == LOW) {                                               //Line_Gliss//
    aTriangle.setPhaseInc(aGliss.next());                                       //Line_Gliss//
    return aTriangle.next();                                                    //Line_Gliss//
  }                                                                             //Line_Gliss//
  while (digitalRead(7) == LOW) {                                               //PWM_Phasing//
    char asig1 = (char)(aPhasor1.next()>>24);                                   //PWM_Phasing//
    char asig2 = (char)(aPhasor2.next()>>24);                                   //PWM_Phasing//
    return ((int)asig1-asig2)/2;                                                //PWM_Phasing//
  }                                                                             //PWM_Phasing//
  while (digitalRead(8) == LOW) {                                               //AMsynth//
    long mod = (128u+ aModulator.next()) * ((byte)128+ aModDepth.next());       //AMsynth//
    int out = (mod * aCarrier.next())>>16;                                      //AMsynth//
    return out;                                                                 //AMsynth//
  }                                                                             //AMsynth//
  while (digitalRead(12) == LOW) {                                              //ReverbTank//
    int synth = bCarrier.phMod((int)bModulator.next()*(150u+aModWidth.next())); //ReverbTank//
    synth *= (byte)aEnvelop.next();                                             //ReverbTank//
    synth >>= 8;                                                                //ReverbTank//
    int arev = reverb.next(synth);                                              //ReverbTank//
    return synth + (arev>>3);                                                   //ReverbTank//
  }                                                                             //ReverbTank//
  return 0;                                                                     //General//
}

void loop(){
  audioHook(); //All//     // This just ouptuts the sounds out of digital pin 9.
}
