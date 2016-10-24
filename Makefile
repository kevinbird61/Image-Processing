TARGET:=bmpreader
GIT_HOOKS := .git/hooks/pre-commit

format:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]

all: format
	gcc main.c -o $(TARGET)

# Gaussian blur
# 1: 5x5 with r,g	,b 3 array
# 2: 5x5 with original structure
gau_blur_tri: $(GIT_HOOKS) format
	gcc main.c -DSPLIT=7 -DGAUSSIAN=1 -o $(TARGET)

gau_blur_ori: $(GIT_HOOKS) format
	gcc main.c -DGAUSSIAN=2 -o $(TARGET)

run:
	# img/wf.bmp => has the 4 element(alpha value)
	./$(TARGET) img/input.bmp output.bmp
	eog output.bmp

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	rm *output.bmp $(TARGET)
