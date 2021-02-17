#include <stdlib.h>
#include "c_speech_features.h"

int
csf_mfcc(const short* aSignal, unsigned int aSignalLen, int aSampleRate,
         csf_float aWinLen, csf_float aWinStep, int aNCep, int aNFilters,
         int aNFFT, int aLowFreq, int aHighFreq, csf_float aPreemph,
         int aCepLifter, int aAppendEnergy, csf_float* aWinFunc,
         csf_float** aMFCC)
{
  int i, j, k, idx, fidx, didx;
  csf_float* feat;
  csf_float* energy;

  int n_frames = csf_logfbank(aSignal, aSignalLen, aSampleRate, aWinLen, aWinStep,
                              aNFilters, aNFFT, aLowFreq, aHighFreq, aPreemph,
                              aWinFunc, &feat, aAppendEnergy ? &energy : NULL);

  // Allocate an array so we can calculate the inner loop multipliers
  // in the DCT-II just one time.
  double* dct2f = (double*)malloc(sizeof(double) * aNFilters * aNCep);

  // Perform DCT-II
  double sf1 = csf_sqrt(1 / (4 * (double)aNFilters));
  double sf2 = csf_sqrt(1 / (2 * (double)aNFilters));
  csf_float* mfcc = (csf_float*)malloc(sizeof(csf_float) * n_frames * aNCep);
  for (i = 0, idx = 0, fidx = 0; i < n_frames;
       i++, idx += aNCep, fidx += aNFilters) {
    for (j = 0, didx = 0; j < aNCep; j++) {
      double sum = 0.0;
      for (k = 0; k < aNFilters; k++, didx++) {
        if (i == 0) {
          dct2f[didx] = cos(M_PI * j * (2 * k + 1) / (double)(2 * aNFilters));
        }
        sum += (double)feat[fidx+k] * dct2f[didx];
      }
      mfcc[idx+j] = (csf_float)(sum * 2.0 * ((i == 0 && j == 0) ? sf1 : sf2));
    }
  }

  // Free inner-loop multiplier cache
  free(dct2f);

  // Free features array
  free(feat);

  // Apply a cepstral lifter
  if (aCepLifter != 0) {
    csf_lifter(mfcc, n_frames, aNCep, aCepLifter);
  }

  // Append energies
  if (aAppendEnergy) {
    for (i = 0, idx = 0; i < n_frames; i++, idx += aNCep) {
      mfcc[idx] = csf_log(energy[i]);
    }

    // Free energy array
    free(energy);
  }

  // Return MFCC features
  *aMFCC = mfcc;

  return n_frames;
}