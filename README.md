# C everyday

Programming in C every day on [Youtube](https://www.youtube.com/@agnlt) every day of my life until I get a girlfriend.  

# Build
Each algorithm has its own folder. When you build the code, an executable will be created in each algorithm folder. To build everything at once, just run
```console
$ make
```
For example, to test the [stack](./stack/) implementation, run:
```console
$ ./stack/stack
```

# Building programs that use Raylib
You need to have [raylib](https://github.com/raysan5/raylib) installed on your system, or you can put the library files into a folder called `lib`, it will be ignoerd by `.gitignore`. Then you can compile with the following command (it's not in the `Makefile`):
```console
# this is to build the tictactoe app
$ cd tictactoe
$ gcc ./tictactoe.c -o tictactoe -L./lib -lraylib # don't use -L if you have Raylib installed system-wide
```
On Windows don't forget to link with `gdi32` and `winmm`.