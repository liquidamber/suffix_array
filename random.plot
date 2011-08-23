set key top left
plot "time/random_naive.time"       title "Naive"  with linespoints pointtype 7,\
     "time/random_larsson.time"     title "LS"     with linespoints pointtype 7,\
     "time/random_induced_org.time" title "IS"     with linespoints pointtype 7,\
     "time/random_induced.time"     title "IS org" with linespoints pointtype 7
pause -1
