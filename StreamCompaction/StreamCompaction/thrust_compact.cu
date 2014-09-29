
#include <thrust/device_ptr.h>
#include "thrust_compact.h"

// Predicate for Thrust
template <typename T>
struct isGreaterThan5 {
	__host__ __device__ bool operator() (const T& in) {
		return (in > 5);
	}
};


int thrustCompactI(const int* in, const int threshold, int* out, int size) {
	/*
	// Convert the input to thrust formats
	thrust::host_ptr<int> h_ptr_in(in);
	thrust::fill(h_ptr_in, h_ptr_in+N, )
	thrust::host_ptr<int> h_ptr_out(out);

	// Copy data to the device
	thrust::device_vector<int> d_vec_in = h_vec_in;
	thrust::device_vector<int> d_vec_out = h_vec_out;

	// Execute stream compaction
	thrust::device_vector<int>::iterator d_it = thrust::copy_if(d_vec_in.begin(), d_vec_in.end(), d_vec_out.begin(), isGreaterThan5<int>());

	// Copy the result vector back
	h_vec_out = d_vec_out;
	//thrust::host_vector<int>::iterator h_it = d_it;
	
	// Convert the output back from thrust formats
	out = thrust::raw_pointer_cast(&h_vec_out[0]);
	*/
	return 0;//(h_it - h_vec_out.begin());
}
