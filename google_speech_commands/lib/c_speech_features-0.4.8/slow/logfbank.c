#include "c_speech_features.h"

int
csf_logfbank(const short* aSignal, unsigned int aSignalLen, int aSampleRate,
             csf_float aWinLen, csf_float aWinStep, int aNFilters, int aNFFT,
             int aLowFreq, int aHighFreq, csf_float aPreemph,
             csf_float* aWinFunc, csf_float** aFeatures, csf_float** aEnergy)
{
  int i, j, idx;
  int n_frames = csf_fbank(aSignal, aSignalLen, aSampleRate, aWinLen, aWinStep,
                           aNFilters, aNFFT, aLowFreq, aHighFreq, aPreemph,
                           aWinFunc, aFeatures, aEnergy);

  for (i = 0, idx = 0; i < n_frames; i++) {
    for (j = 0; j < aNFilters; j++, idx++) {
      (*aFeatures)[idx] = csf_log((*aFeatures)[idx]);
    }
  }

  return n_frames;
}