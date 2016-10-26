CC := gcc
CFLAGS := -msse2 -msse3 -msse4 --std gnu99 -O0
OBJS := gaussian.o mirror.o
TARGET := bmpreader
GIT_HOOKS := .git/hooks/pre-commit

format:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]

%.o: %.c
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

gau_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DPERF=1 -DGAUSSIAN=31 -o $(TARGET) main.c

perf_time: gau_all
	perf stat -r 10 -e cache-misses,cache-references \
	./$(TARGET) img/input.bmp output.bmp 1 > exec_time.log

run:
	@ # img/wf.bmp => has the 4 element(alpha value)
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	./$(TARGET) img/input.bmp output.bmp $$TIMES
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	rm *output.bmp runtime.png $(TARGET) *.log *.o
