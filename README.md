# Prime Stream Processor

Description
This program is intended for the real-time handling of a continuous data flow. It produces a sequence of random numbers and effectively tallies the prime numbers within that sequence through a specialized prime-counting algorithm. This capability is especially beneficial in situations such as processing data from security cameras, where rapid intrusion detection is essential.

Compilation
To run this task you should run "make".
Then run: "time ./randomGenerator 10 10000000 | ./primesCounter"
And then run: "time ./randomGenerator 10 10000000 | ./myPrimCounter"

