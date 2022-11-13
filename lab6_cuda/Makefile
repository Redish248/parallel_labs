all: lab6

lab6: main.cu
	nvcc main.cu -o lab6

lab6_print: lab6_print.cu
	nvcc lab6_print.cu -o lab6p

clean:
	rm -f lab6 lab6p