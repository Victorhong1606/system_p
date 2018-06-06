.SUFFIXES:.c .o  
  
CC=gcc  
  
SRCS1=congaline1.c  
OBJS1=$(SRCS1:.c=.o)  
EXEC1=congaline1  
  
SRCS2=congaline2.c  
OBJS2=$(SRCS2:.c=.o)  
EXEC2=congaline2  
  
start: $(OBJS1) $(OBJS2)  
	$(CC) -o $(EXEC1) $(OBJS1)  
	$(CC) -o $(EXEC2) $(OBJS2)   
  
.c.o:  
	$(CC) -Wall -g -o $@ -c $<  
  
clean:  
	rm -rf $(EXEC1) $(OBJS1)  
	rm -rf $(EXEC2) $(OBJS2) 