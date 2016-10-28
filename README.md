# Image-Processing
Using image processing to test some interest expriment.

# Usage
- `gaussian.c` : guassian blur function implement here.
   - `naive_gaussian_blur_5`:implement gaussian blur with 5x5 weight matrix in 3 seperated color buffers(split)
   - `naive_gaussian_blur_5_original`:implement gaussian blur with 5x5 weight matrix in original data structure
   - `sse_gaussian_blur_5_tri`:implement sse gaussian blur with 5x5 weight matrix in split structure
   - `unroll_gaussian_blur_5_tri`:implement gaussian blur with 5x5 weight matrix in split structure and loop unrolling
   - `unroll_gaussian_blur_5_ori`:implement gaussian blur with 5x5 weight matrix in original structure and loop unrolling
   - `pt_gaussian_blur_5_tri`:implement gaussian blur with 5x5 weight matrix in split structure and loop unrolling (pthread)
- `mirror.c` : image mirror function implement here.
- `main.c` : using #define macro to choose which code section should execute.
- `Makefile` : using `-D` to do different implementation
   - `GAUSSIAN` : doing gaussian blur , and using the value to switch mode.
- `execute.sh` : let user edit the argument(with "enter = default") , call by make run , depend on with type of executed file that user compile.
- `scripts/plot_time.gp` : gnuplot script.

# Run
- `make` : get the original data structure (RGBTRIPLE) to operate.
- Individual mode:
   - `make gau_blur_tri` : get the split data and do 5x5 gaussian blur implementation.
   - `make gau_blur_ori` : get the original structure and do 5x5 gaussian blur implementation.
   - `make gau_blur_sse_tri` : get the split data and do 5x5 gaussian blur implementation with SSE instruction set.
   - `make gau_blur_unr_tri` : get the split data and do 5x5 gaussian blur implementation with loop unrolling.
   - `gau_blur_unr_ori` :  get the original structure and do 5x5 gaussian blur implementation with loop unrolling.
   - `gau_all` : let all type of function be compile
- Run/check performance:
   - `make run` : run the program and get and show the image.
   - `make perf_time` : run the program with all function execution, and output the execution times.
