set term png 
set out "largest.png"

set xla "Density"
set yla "The size of the largest cluster"
unset key
set xtics 0.04
set ytics 0.2
p "serial.dat" u 1:2:4 w e pt 6 lc 1
