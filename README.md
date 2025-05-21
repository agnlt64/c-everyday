# C everyday

Programming in C every day on [Youtube](https://www.youtube.com/@agnlt) every day of my life until I get a girlfriend.

# Actually, it's over
For several reasons, I can't keep on doing this everyday. So I'm stopping this now, before I get completely borded out of it. It was fun while it lasted! But now it's time to focus on something else. I won't make this repo an archive, because maybe one day I'll get back to it.

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

# Building game mechanics
You still need Raylib, but you will have to build each project by hand because I'm too lazy to write a custom Makefile. To run the shooting mechanic, use:
```console
$ cd game_mechanics/shooting
$ gcc shooting.c -o shooting -lraylib # or -L../lib if you don't have Raylib system-wide
```