.PHONY: all
all: generator primesCounter myPrimeCounter 

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primesCounter.c
	gcc -o primesCounter primesCounter.c

myPrimesCounter: myPrimeCounter.c
	gcc -o myPrimeCounter myPrimeCounter.c -lpthread -lm


.PHONY: clean
clean:
	-rm randomGenerator primesCounter myPrimeCounter 2>/dev/null






# .PHONY: all
# all: generator primeCounter

# generator:  generator.c
# 	gcc -o randomGenerator generator.c

# primeCounter:	primeCounter.c
# 	gcc -o primeCounter primeCounter.c

# .PHONY: clean
# clean:
# 	-rm randomGenerator primeCounter 2>/dev/null