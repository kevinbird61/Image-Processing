reset
set xlabel "第幾次執行次數"
set ylabel "time(ms)"
set title "execution time"
set term png enhanced font 'Consolas,10'
set output 'runtime_new.png'

plot "exec_time.log" using 1 with linespoints title "sse original pthread", \
     "exec_time.log" using 2 with linespoints title "sse split", \
     "exec_time.log" using 3 with linespoints title "sse original", \
     "exec_time.log" using 4 with linespoints title "sse prefetch original", \
     "exec_time.log" using 7 with linespoints title "pthread unroll split"
