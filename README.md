# itercpp

Writing iterator in C++

## Development

Make `shell` scripts into an executable

```
chmod +x ./build.sh
```

```
chmod +x ./run.sh
```

Run

```
./run.sh iter.cpp
```

Build

```
./build.sh iter.cpp
```

Use `bear` to generate the `compile_commands.json` for the lsp configuration.

```
bear -- ./build.sh iter.cpp
```
