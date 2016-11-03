CC := gcc
CFLAGS := -msse2 -msse3 -msse4 --std gnu99 -O0
OBJS := gaussian.o mirror.o hsv.o
HEADER := gaussian.h mirror.h hsv.h
TARGET := bmpreader
GIT_HOOKS := .git/hooks/pre-commit

format:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

main.o: main.c $(HEADER)
	$(CC) -std=gnu99 -c -DPERF=1 -DGAUSSIAN=1 -DMIRROR=0 -DHSV=0 -o $@ $<

vmain.o: main.c $(HEADER)
	$(CC) -c -DPERF=1 -DGAUSSIAN=1 -DMIRROR=0 -DHSV=0 -g -o $@ $<

# Gaussian blur
gau_all: $(GIT_HOOKS) format $(OBJS) main.o
	$(CC) $(CFLAGS) $(OBJS) main.o -o $(TARGET) -lpthread

mirror_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=1 -DHSV=0 -o $(TARGET) main.c

hsv: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=0 -DHSV=1 -o $(TARGET) main.c -fopenmp

perf_time: gau_all
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	read -p "Enter the thread number: " THREADS; \
	read -p "Enter the times on perf analysis: " PERFT; \
	perf stat -r $$PERFT -e cache-misses,cache-references \
	./$(TARGET) img/input.bmp output.bmp $$TIMES $$THREADS > exec_time.log
	gnuplot scripts/plot_time.gp

run:
	bash execute.sh $(TARGET) img/input.bmp output.bmp;
	eog output.bmp

valgrind: $(GIT_HOOKS) format $(OBJS) vmain.o
	$(CC) $(CFLAGS) $(OBJS) vmain.o -o $(TARGET) -lpthread
	valgrind --leak-check=full ./$(TARGET) img/input.bmp output.bmp 1 4

$(GIT_HOOKS):
	@scripts/install-git-hooks

clean:
	$(RM) *output.bmp runtime.png $(TARGET) *.log *.o
