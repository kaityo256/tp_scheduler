set term png
set out "order.png"

set xla "Temperature"
set yla "Magnetization"
set xtics 0.2
set ytics 0.2
unset key
p "serial.dat" u 1:2:4 w e lc 1 pt 6
