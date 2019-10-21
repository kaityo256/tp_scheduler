set term png 
set out "crossing.png"

set xla "Density"
set yla "Crossing Probability"
unset key
set xtics 0.04
set ytics 0.2
p "serial.dat" u 1:5:7 w e pt 6 lc 1
