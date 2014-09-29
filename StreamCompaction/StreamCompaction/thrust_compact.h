#ifndef THRUST_COMPACT_H_
#define THRUST_COMPACT_H_

// Thrust Dependencies
#include <thrust/copy.h>

/* Perform compaction using GPU with Thrust libraries */
/* @param in Input array to scatter */
/* @param threshold Value to threshold the input array on (using >) */
/* @param out Output array of condensed data meeting the threshold */
/* @param size The integer size of in */
/* @return The integer size of out */
int thrustCompactI(const int* in, const int threshold, int* out, int size);

#endif // THRUST_COMPACT_H_
