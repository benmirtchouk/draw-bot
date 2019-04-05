# draw-bot
A bot to draw an image with mouse clicks

## Example application:
Draw Something - Facebook messenger game. [Example](https://youtu.be/1GLR66jbtCM)

## Extra info
- The image it reads from should be in the same directory and named "img.bmp" (obviously must be a bitmap img).
- The `quality` variable controls how many pixels are drawn, 100-200 tends to work best but you can experiment.
- The mouse clicks are controled by Windows.h, so this is Windows only.
- As with all applications that steer the mouse (and click thousands of times), be careful before setting an extremely high quality or running with a random window in the foreground. Ctrl-Alt-Del and Ctrl-C are useful.