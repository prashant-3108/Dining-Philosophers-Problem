# Dining Philosophers Problem

![alt text](dp.jpg)

### Problem Statement

The dining philosophers problem states that there are 5 philosophers sharing a circular table and they eat and think alternatively. A hungry philosopher may only eat if there are both chopsticks available. Otherwise a philosopher puts down their chopstick and begin thinking again.

### Deadlock Situation - An Issue

When a philosopher wants to eat the rice, he will wait for the chopstick at his left and picks up that chopstick. But if all five philosophers are hungry simultaneously, and each of them pickup one chopstick, then a deadlock situation occurs because they will be waiting for another chopstick forever.

### Deadlock Free Solution

This is an algorithm to demonstrate Dining philosophers' problem solution using threads and mutex variables with the Monitor (Deadlock Free) Approach.

Program takes input from command lines the number of philosophers and the times they get hungry and eat.

Threads are used in the programs where each thread represents the while dining activity of that philosopher till he eats the given number of times. If all Philosophers eat maximum times allowed to eat then they stop and program exits.

    int num_phil = Total number of Philosophers;
    int freq = Total number of times allowed to eat;

We create threads for each process calling the dining function and join them in the main. Initially all philosophers are thinking.

Then we create threads for all the philosophers to dine and join them.

We are using a mutex to ensure mutaual exclusion and progress among the threads and using a conditional variable for each philosopher for the waiting conditions which can occur when a neighbour is having chopsticks and hence current philosopher has to wait.

While dining for a particular frequency:
    Pickup Chopstick() -- change state to hungry and wait till both chopsticks philosopher needs are free.
    Eat time ..
    PutDown Chopsticks() -- finished eating change state to thinking and checking if its neighbours are in waiting state then they can eat.

Freeing all the memory including dynamic arrays and conditional variable array.

### Instructions For Running the Algorithm

1. Open Linux Terminal in this Repository.	[Hold Right Mouse + E]
2. Run Following commands to make and run the Executable file: 
<br>

	make
	./dp num_philosophers num_times_eaten

3. After Exit, run the following command to clean the executable file:
<br>
	
	make clean



