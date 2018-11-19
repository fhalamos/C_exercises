MPCS 51100 PSet 3

Student Name: Felipe Alamos
Student Email: fhalamos@uchicago.edu

DISCUSSION

#P1
First argument when calling program shall be size of matriz. ex: ./p1 10.
For matriz nxn n=2000, nthreads=4
Work time matrix_product_serial:37.480709

#P2
First argument when calling program shall be size of matriz, second argument shall be number of threads. ex: ./p2 10 4

For matriz nxn n=2000, nthreads=4
Work time matrix_product_serial:31.842803
Work time matrix_product_pthreads:7.703417

#P3
First argument when calling program shall be size of matriz, second argument shall be number of threads. ex: ./p3 10 4

For matriz nxn n=2000, nthreads=4
Work time matrix_product_serial:40.016793
Work time matrix_product_openmp:24.944119

#P4
First argument when calling program shall be size of matriz. ex: ./p4 10.
Speedup results and graph in p4.pdf

#P5
Work time julia_set_serial:0.848282
Run generate_graph.py with filename p5_pixels.txt.
Image plot in p5.pdf

#P6
For n=1000, nthread=4
Work time julia_set_serial:0.956382
Work time julia_set_pthreads:0.267415
Run generate_graph.py with filename p6_pixels.txt
Image plot in p6.pdf

#P7
For n=1000, nthread=4
Work time julia_set_serial:0.890299
Work time julia_set_openmp (with optimal chuncksize = 1): 0.233022
Run generate_graph.py with filename p7_pixels.txt
Image plot in p7.pdf
An analysis to find optimal chuncksize is included in p7_chuncksize analysis.pdf

#P8
Serial:0.863942
nthreads	pthreads	openmp
1			0.561112	0.875063
2			0.281143	0.221540
3			0.228212	0.151797
4			0.178105	0.116643

Plot and analysis found in p8.pdf

#P9
->Serial method
Pi obtained: 		3.139188
Error: 				0.002405, 0.0765%
Time to compute: 	0.012798 sec

->With atomic
Pi obtained: 		3.141164
Error: 				0.000429, 0.0136%
Time to compute: 	0.041153 sec

->With critical
Pi obtained: 		3.143024
Error: 				0.001431, 0.0456%
Time to compute: 	0.062061 sec

->Manual reduction a
Pi obtained: 		3.140912
Error: 				0.000681, 0.0217%
Time to compute: 	0.007846 sec

->Manual reduction b
Pi obtained: 		3.139660
Error: 				0.001933, 0.0615%
Time to compute: 	0.006802 sec

->Automatic reduction
Pi obtained: 		3.138724
Error: 				0.002869, 0.0913%
Time to compute: 	0.005279 sec


#P10
Analysis found in p10.pdf


#P11
Results, plot and discussion found in p11.pdf


-----
#REFERENCES
-P2
https://stackoverflow.com/questions/2962785/c-using-clock-to-measure-time-in-multi-threaded-programs

-P9
https://stackoverflow.com/questions/43151361/how-to-create-thread-safe-random-number-generator-in-c-using-rand-r

-P11
https://gist.github.com/rtv/4989304#file-cond-c