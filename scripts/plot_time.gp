reset
set xlabel "第幾次執行次數"
set ylabel "time(ms)"
set title "execution time"
set term png enhanced font 'Consolas,10'
set output 'runtime_naive.png'

plot "exec_time.log" using 10 with linespoints title "naive split" , \
     "exec_time.log" using 11 with linespoints title "naive original", \
     "exec_time.log" using 5 with linespoints title "unroll split" , \
     "exec_time.log" using 6 with linespoints title "unroll original" , \
     "exec_time.log" using 8 with linespoints title "unroll 1D-filter original", \
     "exec_time.log" using 9 with linespoints title "unroll expand split"
