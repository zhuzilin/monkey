# monkey
A C++ version [monkey](https://monkeylang.org/) language interpreter. From [Write An Interpreter In Go](https://interpreterbook.com/).
## Usage
You can use the vscode config to build and run the interpreter. Or
```bash
g++ -std=c++11 repl.cpp src/*.cpp -o monkey --debug
./monkey
```
And `repl.cpp` is the REPL(Read-Eval-Print Loop) main function, to only use parser or lexer, you can change to `rppl.cpp` or `rlpl.cpp`.

## TODOs
- Add error handling for lexer.
- Add garbage collection (maybe just using the share_ptr).
- Add array and dictionary support.
- Add buildin functions.
