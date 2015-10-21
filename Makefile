TARGET = domi
CC = gcc
OBJS = lex.yy.o y.tab.o create.o main.o init.o computer.o var.o local_func.o

#编译得到domi
$(TARGET) : $(OBJS) domi.h
	$(CC) $(OBJS) -o $@
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h domi

y.tab.h y.tab.c: domi.y domi.h
	yacc -d domi.y

lex.yy.c: domi.l domi.h
	lex domi.l
y.tab.o: y.tab.c domi.h
	$(CC) -g -c $*.c
lex.yy.o: lex.yy.c domi.h y.tab.h
	$(CC) -g -c $*.c
