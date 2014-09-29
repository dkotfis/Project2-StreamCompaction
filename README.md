Project-2
=========

A Study in Parallel Algorithms : Stream Compaction

What I have Done:
 1.) Scan
	- CPU
	- Naive GPU
	- One Block GPU
	- N Block GPU
 2.) Scatter
	- CPU
	- GPU (w/ One Block)
 3.) Compact
	- CPU
	- GPU (w/ One Block)

What I have Started:
 - Compaction using Thrust. I put this to the side once I had trouble with device/host vectors and lots of crashing my system. The thrust documentation is pretty confusing, and the compiler wasn't catching enough of my dumb mistakes to help me get anywhere.
 - N Block Scan Optimizations. I suspect that there is a better way to do this than my brute-force-ish approach that doesn't seem like it utilizes parallelism very well.

Known Bugs:
 - Naive Scan and N Block Scan come up short by 1 depth iteration consistently. I can't find the source of this, since the calculation is the same as in One Block. It seems to be imprecision in the floating point operations on the GPU.
 - The last element of Compact can be pretty hit or miss. I use extrinsic calculations for all of the intermediary steps, so the value of the last element isn't maintained.

What I have learned:
 - Start earlier. Even though Patrick projected this to take 3-5 hours, putting it off until the weekend wasn't wise. I spent upwards of 15 hours on it and didn't get a chance to do much analysis or optimization.
 - Templates + CUDA can be rough. I tried templating all of my stuff so it could be used with arrays of different data types. A lot of times this resulted in strange and unhelpful compiler errors that not even the internet knew anything about.

Analysis:
 - CPU Performance - I've uploaded a screenshot of console output showing CPU speeds for prefix sum for a wide variety of array lengths. It seems as though it is O(1) for these calculations, at least up to the resolution of timing measurement. More analysis could show whether there is a point where this acutally breaks down. Note: all of these are using arrays of floats, not ints.
 - Naive Scan Performance - I've uploaded a screenshot of console output showing GPU speeds for scan matching those for the CPU evaluation. For all cases, it is at least 2 orders of magnitude slower than the CPU implementation, though I didn't average the time over 100+ runs which would allow the GPU to start to speed up. After lengths 100,000 and up, the timing seems to go to O(n) once we start to lose enough time going back and forth to global memory.
