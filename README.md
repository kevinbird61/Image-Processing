# Image-Processing
Using image processing to test some interest expriment.

# Run
- Way 1 (Base on `Makefile + execute.sh`)
  - `make` : get the original data structure (RGBTRIPLE) to operate.
  - avaliable target:
     - `gau_all` : run all types of gaussian blur functions on image.
     - `mirror_all` : run all types of mirror functions on image.
     - `hsv` : run all types of hsv functions on image.
  - Run/check performance:
     - `make run` : run the program and get and show the image.
     - `make perf_time` : run the program with all function execution, and output the execution times.
- Way 2 (Base on `image_process.sh`)
  - Using shell script to choose compile arguments
  - `bash image_process.sh [-o ... ] [--option ... ]`
  - `short option: -o`
    - -a : compile with all gaussian function (= gau_all)
    - -e : use when compile with ARM environment (**TODO**)
    - -v : use when want to compile with valgrind (Can't use with perf)
    - -t : use when you only want to compile and run the test module part.
    - -m : compile with all mirror function
    - -h : compile with all hsv function
    - -s : use when you want to compile with strictly option (`-Wall -pedantic`)
    - -g *N* : choose specific gaussian function to run, using `N` to distinguish. (**TODO**)
  - `long option: --option`
    - --perf *N*: compile and apply `N` times perf on program.
    - --clean : same function as `make clean`
    - --help : list usage

### Another Usage
- `execute.sh` : let user edit the argument(with "enter = default") , call by make run , depend on with type of executed file that user compile.
- `scripts/plot_time.gp` : gnuplot script.
