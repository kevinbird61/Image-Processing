TARGET:=bmpreader

all:
	gcc main.c -o $(TARGET)

split:
	gcc main.c -DSPLIT=7 -o $(TARGET)

# Gaussian blur
# 1: 5x5 with r,g,b 3 array
gau_blur_tri:
	gcc main.c -DSPLIT=7 -DGAUSSIAN=1 -o $(TARGET)

run:
	# img/wf.bmp => has the 4 element(alpha value)
	./$(TARGET) img/input.bmp output.bmp
	eog output.bmp

clean:
	rm *output.bmp $(TARGET)
