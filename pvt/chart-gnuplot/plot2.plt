#!/usr/bin/gnuplot

set termoption enhanced
set terminal png size 1400,900 font "Arial, 24"
set output "grath2.png"
set style line 1 lc rgb "0x1500ff" lt 3 lw 6 pt 5 ps 1
set style line 2 lc rgb "0x00ff1a" lt 3 lw 6 pt 7 ps 1
set border lw 2
set grid
set key top left
set xlabel "Threads"
set ylabel "Speedup" rotate by 90
set xtics 1
set ytics 1
set format x "%6.0f"
set format y "%1.0f"
set yrange [1:5]
set xrange [2:4]
plot "rezult2.txt" using 1:2 title "N = 100000000" with linespoints ls 2, \
"rezult.txt" using 1:3 title "Linear speedup" with linespoints ls 1, \