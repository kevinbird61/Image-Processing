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

# Gaussian blur
# 1: 5x5 with split r,g	,b 3 array
# 2: 5x5 with original structure
# 4: 5x5 with sse split structure
# 8: 5x5 with	unroll split structure
# 16: 5x5 with unroll original structure
gau_blur_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=1 -o $(TARGET) main.c

gau_blur_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=2 -o $(TARGET) main.c

gau_blur_sse_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=4 -o $(TARGET) main.c

gau_blur_unr_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=8 -o $(TARGET) main.c

gau_blur_unr_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=16 -o $(TARGET) main.c

gau_blur_ptunr_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=32 -o $(TARGET) main.c -lpthread

gau_blur_sse_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=64 -o $(TARGET) main.c

gau_blur_sse_ori_prefetch: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=128 -o $(TARGET) main.c

gau_blur_ptsse_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=256 -o $(TARGET) main.c -lpthread

gau_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DPERF=1 -DGAUSSIAN=511 -o $(TARGET) main.c -lpthread

mirror_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=1 -DHSV=0 -o $(TARGET) main.c

hsv: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=0 -DHSV=1 -o $(TARGET) main.c -fopenmp

perf_time: gau_all
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	read -p "Enter the thread number: " THREADS; \
	perf stat -r 100 -e cache-misses,cache-references \
	./$(TARGET) img/input.bmp output.bmp $$TIMES $$THREADS > exec_time.log
	gnuplot scripts/plot_time.gp

run:
	bash execute.sh $(TARGET) img/input.bmp output.bmp;
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks

clean:
	$(RM) *output.bmp runtime.png $(TARGET) *.log *.o
