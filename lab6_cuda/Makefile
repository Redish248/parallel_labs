all: lab6 print time

lab6: main.cu
	nvcc main.cu -o lab6

print: lab6_print.cu
	nvcc lab6_print.cu -o lab6p

time: lab6_time.cu
	nvcc lab6_time.cu -o lab6t

clean:
	rm -f lab6 lab6p lab6t