TARGET = domi
CC=gcc
OBJS = \
  lex.yy.o\
  y.tab.o\
  main.o\
  interface.o\
  create.o\
  execute.o\
  eval.o\
  string.o\
  string_pool.o\
  util.o\
  native.o\
  error.o\
  error_message.o\
  ./MEM/mem.o\
  ./DEG/dbg.o
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = \

$(TARGET):$(OBJS)
	cd ./MEM; $(MAKE);
	cd ./DEG; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
y.tab.h : domi.y
	bison --yacc -dv domi.y
y.tab.c : domi.y
	bison --yacc -dv domi.y
lex.yy.c : domi.l domi.y y.tab.h
	flex domi.l
y.tab.o: y.tab.c domi.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c domi.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
./memory/mem.o:
	cd ./MEM; $(MAKE);
./debug/dbg.o:
	cd ./DEG; $(MAKE);
############################################################
create.o: create.c MEM.h DBG.h domi.h DM.h DM_dev.h
error.o: error.c MEM.h domi.h DM.h DM_dev.h
error_message.o: error_message.c domi.h MEM.h DM.h DM_dev.h
eval.o: eval.c MEM.h DBG.h domi.h DM.h DM_dev.h
execute.o: execute.c MEM.h DBG.h domi.h DM.h DM_dev.h
interface.o: interface.c MEM.h DBG.h domi.h DM.h DM_dev.h
main.o: main.c DM.h MEM.h
native.o: native.c MEM.h DBG.h domi.h DM.h DM_dev.h
string.o: string.c MEM.h domi.h DM.h DM_dev.h
string_pool.o: string_pool.c MEM.h DBG.h domi.h DM.h DM_dev.h
util.o: util.c MEM.h DBG.h domi.h DM.h DM_dev.h
