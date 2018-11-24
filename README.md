# Eight Queens Puzzle

In the Eight Queens puzzle, you have to place 8 queens in a chess board in such a way that they do not threat each other

## Compilation

```
cc eight-queens.c -o eight-queens $(pkg-config  allegro --libs --cflags)
```

`$(pkg-config allegro --libs --cflags)`  resolves to `-lalleg` in `Ubuntu 16.04`

## Running

```
./eight-queens
```
