CC := gcc
CFLAGS := -g --std gnu99 -O0
SSEFLAGS := -msse2 -msse3 -msse4
TARGET := bmpreader
GIT_HOOKS := .git/hooks/pre-commit

format:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]

# Gaussian blur
# 1: 5x5 with r,g	,b 3 array
# 2: 5x5 with original structure
gau_blur_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DSPLIT=7 -DGAUSSIAN=1 -o $(TARGET)

gau_blur_ori: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DGAUSSIAN=2 -o $(TARGET)

gau_blur_sse_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) $(SSEFLAGS) main.c -DGAUSSIAN=3 -o $(TARGET)

gau_blur_unr_tri: $(GIT_HOOKS) format
	$(CC) $(CFLAGS) main.c -DSPLIT=7 -DGAUSSIAN=4 -o $(TARGET)

run:
	@ # img/wf.bmp => has the 4 element(alpha value)
	@read -p "Enter the times you want to execute Gaussian blur on the input picture:" TIMES; \
	./$(TARGET) img/input.bmp output.bmp $$TIMES
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	rm *output.bmp $(TARGET)
