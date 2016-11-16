CC := gcc
CFLAGS := -msse2 -msse3 -msse4 --std gnu99 -O0
PFLAGS := -lpthread
ARM_CC ?= arm-linux-gnueabihf-gcc-5
ARM_CFLAGS = -c -g -Wall -Wextra -Ofast -mfpu=neon
ARM_LDFLAGS = -Wall -g -Wextra -Ofast
OBJS := gaussian.o mirror.o hsv.o
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
	$(CC) $(CFLAGS) $(OBJS) -DARM -DGAUSSIAN=1 -o $(TARGET) main.c $(PFLAGS)

gau_blur_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=2 -o $(TARGET) main.c $(PFLAGS)

gau_blur_sse_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=4 -o $(TARGET) main.c $(PFLAGS)

gau_blur_unr_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=8 -o $(TARGET) main.c $(PFLAGS)

gau_blur_unr_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=16 -o $(TARGET) main.c $(PFLAGS)

gau_blur_ptunr_tri: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=32 -o $(TARGET) main.c $(PFLAGS)

gau_blur_sse_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=64 -o $(TARGET) main.c $(PFLAGS)

gau_blur_sse_ori_prefetch: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=128 -o $(TARGET) main.c $(PFLAGS)

gau_blur_ptsse_ori: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=256 -o $(TARGET) main.c $(PFLAGS)

#gau_blur_arm_tri: $(GIT_HOOKS) main.c
#	$(ARM_CC) $(ARM_CFLAGS) $(OBJS) -DARM -o $(TARGET) main.c
#	$(ARM_CC) $(ARM_LDFLAGS) -o main_arm main_arm.o

gau_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DPERF=1 -DGAUSSIAN=511 -o $(TARGET) main.c $(PFLAGS)

mirror_all: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=1 -DHSV=0 -o $(TARGET) main.c $(PFLAGS)

hsv: $(GIT_HOOKS) format main.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -DGAUSSIAN=0 -DMIRROR=0 -DHSV=1 -o $(TARGET) main.c -fopenmp

gau_blur_neon_ori: $(GIT_HOOKS) format main.c
	arm-linux-gnueabihf-gcc-5 -O0 -c -g -Wall -Wextra -Ofast -mfpu=neon -DARM -o gaussian.o gaussian_arm.c 
	arm-linux-gnueabihf-gcc-5 --std gnu99 -DARM -DGAUSSIAN_ARM -DGAUSSIAN=0 -Wall -Wcomment -O3 -g -Wextra -Ofast gaussian.o -o $(TARGET) main.c

mirror_neon_tri: $(GIT_HOOKS) format main.c gaussian.o
	arm-linux-gnueabihf-gcc-5 -O0 -c -g -Wall -Wextra -Ofast -mfpu=neon -DARM -o mirror.o mirror_arm.c 
	arm-linux-gnueabihf-gcc-5 --std gnu99 -DARM -DMIRROR_ARM -DMIRROR=0 -Wall -Wcomment -O0 -g -Wextra -Ofast mirror.o -o $(TARGET) main.c

#perf_time: gau_blur_neon_ori
#	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
#	read -p "Enter the thread number: " THREADS; \
#	perf stat -r 100 -e cache-misses,cache-references \
#	./$(TARGET) img/input.bmp output.bmp $$TIMES $$THREADS > exec_time.log
#	gnuplot scripts/plot_time.gp
perf_time: gau_blur_neon_ori
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	read -p "Enter the thread number: " THREADS; \
	./$(TARGET) img/input.bmp output.bmp $$TIMES $$THREADS > exec_time.log
	gnuplot scripts/plot_time.gp

run:
	bash execute.sh $(TARGET) img/input.bmp output.bmp;
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks

clean:
	$(RM) *output.bmp runtime.png $(TARGET) *.log *.o
