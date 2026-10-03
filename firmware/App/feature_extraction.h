/**
 * @file feature_extraction.h
 * @brief Feature extraction from sensor buffers
 * @author Chetan I K
 * @date October 2026
 */

#ifndef FEATURE_EXTRACTION_H
#define FEATURE_EXTRACTION_H

#include <stdint.h>

#define NUM_FEATURES 8

#define FEAT_MEAN        0
#define FEAT_VARIANCE    1
#define FEAT_SLOPE       2
#define FEAT_RMS         3
#define FEAT_EWMA        4
#define FEAT_CUSUM       5
#define FEAT_RATE        6
#define FEAT_RANGE       7

void extract_features(const float *buffer, int size, float *features);
float compute_ewma(const float *buffer, int size, float alpha);
float compute_cusum(const float *buffer, int size, float k, float h);

#endif /* FEATURE_EXTRACTION_H */