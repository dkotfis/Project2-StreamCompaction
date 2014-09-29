
// C Dependencies
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>

// Project Dependencies
#include "prefix_sum.h"
#include "timing_utils.h"
#include "thrust_compact.h"

//Global data for run settings
int LENGTH = 100;
const int MAX_LENGTH = 100;
const int MAX = 10;
const bool TIMING = true;
const bool PRINT_DATA = true;

template <typename T>
void cpuPrefixSum(const T* in, T* out, int size) {
	T sum = static_cast<T>(0.0f);
	for (int i = 0; i < size; i++) {
		out[i] = sum;
		sum +=	in[i];
	}
}

template<typename T>
void cpuScatter(const T* in, const T threshold, int* out, int size) {
	int* temp = (int*) malloc(size*sizeof(int));
	for (int i = 0; i < size; i++) {
		temp[i] = static_cast<int>(in[i] > threshold);
	}
	cpuPrefixSum<T>(temp, out, size);
	free(temp);
}

template<typename T>
int cpuCompact(const T* in, const T threshold, T* out, int size_in) {
	int* temp = (int*) malloc(size_in*sizeof(int));
	cpuScatter<T>(in, threshold, temp, size_in);
	int result = temp[size_in-1];
	int current = result;
	for (int i = (size_in-1); i >= 0; i--) {
		if (temp[i] == current) {
			out[current] = in[i];
			current--;
		}
	}
	return result+1;
}

void printResults(float* data, int size) {
	fprintf(stdout, "[");
	for (size_t i = 0; i < size; i++) {
		if (i != (size-1)) {
			fprintf(stdout, "%f, ", data[i]);
		} else {
			fprintf(stdout, "%f", data[i]);
		}
	}
	fprintf(stdout, "]\n");
}

void printResults(int* data, int size) {
	fprintf(stdout, "[");
	for (size_t i = 0; i < size; i++) {
		if (i != (size-1)) {
			fprintf(stdout, "%d, ", data[i]);
		} else {
			fprintf(stdout, "%d", data[i]);
		}
	}
	fprintf(stdout, "]\n");
}

void printResults(bool* data, int size) {
	fprintf(stdout, "[");
	for (size_t i = 0; i < size; i++) {
		if (i != (size-1)) {
			fprintf(stdout, "%d, ", data[i] ? 1 : 0);
		} else {
			fprintf(stdout, "%d", data[i] ? 1 : 0);
		}
	}
	fprintf(stdout, "]\n");
}

int main(int argc, char** argv) {

	int new_length;

	for (LENGTH = 10; LENGTH <= MAX_LENGTH; LENGTH *= 10) {

		// Seed the random number generator
		srand(time(NULL));

		// Initialize data (randomly)
		int *start = (int*) malloc(LENGTH*sizeof(int));
		int *end = (int*) malloc(LENGTH*sizeof(int));
		for (int i = 0; i < LENGTH; i++) {
			start[i] = (int) ( (float) rand() / (float) (RAND_MAX/MAX) );
		}
		int *scat_res = (int*) malloc(LENGTH*sizeof(int));
		if (PRINT_DATA) {
			fprintf(stdout, "Input Array:\n");
			printResults(start, LENGTH);
			fprintf(stdout, "-------- \n");
		}

		//Call the CPU implementation
		fprintf(stdout, "CPU Scan:\n");
		if (!TIMING) {
			cpuPrefixSum<int>(start, end, LENGTH);
		} else {
			startTiming();
			cpuPrefixSum(start, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call the naive GPU implementation
		fprintf(stdout, "Naive GPU Scan:\n");
		if (!TIMING) {
			gpuNaivePrefixSumI(start, end, LENGTH);
		} else {
			startTiming();
			gpuNaivePrefixSumI(start, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call the one block GPU implementation
		fprintf(stdout, "One Block GPU Scan:\n");
		if (!TIMING) {
			gpuOneBlockPrefixSumI(start, end, LENGTH);
		} else {
			startTiming();
			gpuOneBlockPrefixSumI(start, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call the multi block GPU implementation
		fprintf(stdout, "Multi Block GPU Scan:\n");
		if (!TIMING) {
			gpuNBlockPrefixSumI(start, end, LENGTH);
		} else {
			startTiming();
			gpuNBlockPrefixSumI(start, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call CPU Scatter
		fprintf(stdout, "CPU Scatter:\n");
		if (!TIMING) {
			cpuScatter<int>(start, 5, scat_res, LENGTH);
		} else {
			startTiming();
			cpuScatter<int>(start, 5, scat_res, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(scat_res, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call GPU Scatter
		fprintf(stdout, "GPU Scatter:\n");
		if (!TIMING) {
			gpuScatterI(start, 5, scat_res, LENGTH);
		} else {
			startTiming();
			gpuScatterI(start, 5, scat_res, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(scat_res, LENGTH);
		}
		fprintf(stdout, "-------- \n");

		//Call CPU Compact
		fprintf(stdout, "CPU Compact:\n");
		if (!TIMING) {
			new_length = cpuCompact<int>(start, 5, end, LENGTH);
		} else {
			startTiming();
			new_length = cpuCompact<int>(start, 5, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, new_length);
		}
		fprintf(stdout, "-------- \n");

		//Call GPU Compact
		fprintf(stdout, "GPU Compact:\n");
		if (!TIMING) {
			new_length = gpuCompactI(start, 5, end, LENGTH);
		} else {
			startTiming();
			new_length = gpuCompactI(start, 5, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, new_length);
		}
		fprintf(stdout, "-------- \n");

		//Call Thrust Compact
		/*
		fprintf(stdout, "Thrust Compact:\n");
		if (!TIMING) {
			new_length = thrustCompactI(start, 5, end, LENGTH);
		} else {
			startTiming();
			new_length = thrustCompactI(start, 5, end, LENGTH);
			float timeTaken = stopTiming();
			fprintf(stdout, "Length: %d, Execution time: %f ms.\n", LENGTH, timeTaken);
		}
		if (PRINT_DATA) {
			printResults(end, new_length);
		}
		fprintf(stdout, "-------- \n");
		*/

		//Free up memory
		free(start);
		free(end);
		free(scat_res);
	}

	while (true) {

	}

	return 0;
}
