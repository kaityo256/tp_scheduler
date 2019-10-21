set term png
set out "binder.png"

set xla "Temperature"
set yla "Binder ratio"
set xtics 0.2
set ytics 0.5
set yra [1:3]
unset key
p "serial.dat" u 1:5:7 w e lc 1 pt 6
