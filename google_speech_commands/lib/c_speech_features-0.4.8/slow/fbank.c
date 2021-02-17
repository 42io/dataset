#include <stdlib.h>
#include <float.h>
#include "c_speech_features.h"

int
csf_fbank(const short* aSignal, unsigned int aSignalLen, int aSampleRate,
          csf_float aWinLen, csf_float aWinStep, int aNFilters, int aNFFT,
          int aLowFreq, int aHighFreq, csf_float aPreemph, csf_float* aWinFunc,
          csf_float** aFeatures, csf_float** aEnergy)
{
  int i, j, k, idx, fidx, pidx;
  csf_float* feat;
  csf_float* fbank;
  csf_float* pspec;
  csf_float* frames;
  csf_float* energy;
  csf_float* preemph = csf_preemphasis(aSignal, aSignalLen, aPreemph);
  int frame_len = (int)round(aWinLen * aSampleRate);
  int frame_step = (int)round(aWinStep * aSampleRate);
  int feat_width = aNFFT / 2 + 1;

  // Frame the signal into overlapping frames
  int n_frames = csf_framesig(preemph, aSignalLen, frame_len, aNFFT,
                              frame_step, aWinFunc, &frames);

  // Free preemphasised signal buffer
  free(preemph);

  // Compute the power spectrum of the frames
  pspec = csf_powspec((const csf_float*)frames, n_frames, aNFFT);

  // Free frames
  free(frames);

  // Store the total energy in each frame
  if (aEnergy) {
    energy = (csf_float*)calloc(sizeof(csf_float), n_frames);
    for (i = 0, idx = 0; i < n_frames; i++) {
      for (j = 0; j < feat_width; j++, idx++) {
        energy[i] += pspec[idx];
      }
      if (energy[i] == 0.0) {
        energy[i] = csf_float_min;
      }
    }
  }

  // Compute the filter-bank energies
  fbank = csf_get_filterbanks(aNFilters, aNFFT, aSampleRate,
                              aLowFreq, aHighFreq);
  feat = (csf_float*)calloc(sizeof(csf_float), n_frames * aNFilters);
  for (i = 0, idx = 0, pidx = 0; i < n_frames;
       i++, idx += aNFilters, pidx += feat_width) {
    for (j = 0, fidx = 0; j < aNFilters; j++) {
      for (k = 0; k < feat_width; k++, fidx++) {
        feat[idx + j] += pspec[pidx + k] * fbank[fidx];
      }
      if (feat[idx + j] == 0.0) {
        feat[idx + j] = csf_float_min;
      }
    }
  }

  // Free fbank
  free(fbank);

  // Free pspec
  free(pspec);

  // Return features and energies
  *aFeatures = feat;
  if (aEnergy) {
    *aEnergy = energy;
  }

  return n_frames;
}