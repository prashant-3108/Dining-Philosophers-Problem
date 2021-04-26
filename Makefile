all: dp 

dp:
	gcc -g -Wall -o dp dining_philosopher.c -pthread

clean:
	$(RM) dp
