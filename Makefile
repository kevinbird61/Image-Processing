CC := gcc
CFLAGS := -msse2 -msse3 -msse4 --std gnu99 -O0
TARGET := bmpreader
GIT_HOOKS := .git/hooks/pre-commit

format:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]

# Gaussian blur
# 1: 5x5 with split r,g	,b 3 array
# 2: 5x5 with original structure
# 4: 5x5 with sse split structure
# 8: 5x5 with	unroll split structure
# 16: 5x5 with unroll original structure
gau_blur_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=1 -o $(TARGET)

gau_blur_ori: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=2 -o $(TARGET)

gau_blur_sse_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=4 -o $(TARGET)

gau_blur_unr_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=8 -o $(TARGET)

gau_blur_unr_ori: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=16 -o $(TARGET)

perf_time:
	@ # All time output in one line
	$(CC) $(CFLAGS) main.c -DPERF=1 -DSPLIT=7 -DGAUSSIAN=31 -o $(TARGET)
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	@ #3>exec_perf.log \
	perf stat -r 100 -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	--log-fd 3 \
	./$(TARGET) img/input.bmp output.bmp $$TIMES \ > exec_time.log

run:
	@ # img/wf.bmp => has the 4 element(alpha value)
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	./$(TARGET) img/input.bmp output.bmp $$TIMES
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	rm *output.bmp runtime.png $(TARGET) *.log
