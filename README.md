# monkey
A C++ version [monkey](https://monkeylang.org/) language interpreter. From [Write An Interpreter In Go](https://interpreterbook.com/).
With additional mark-and-sweep garbage control.
## Usage
You can use the vscode config to build and run the interpreter. Or
```bash
> g++ -std=c++11 main.cpp src/*.cpp -o monkey --debug
> ./monkey test.mk
> Integer
> 20
```
The `test.mk` is a gcd function.
```js
let a = 100;
let b = 120;

// gcd function
let gcd = fn (a, b) {
	if(b == 0) {
		return a;
	} else {
		return gcd(b, a%b);
	}
}

gcd(a, b);
```
And `repl.cpp` is the REPL(Read-Eval-Print Loop) main function, to only use parser or lexer, you can change to `rppl.cpp` or `rlpl.cpp`.

## TODOs
* [ ] Add error handling for lexer.
* [x] Add garbage collection (maybe just using the share_ptr).
* [ ] Add array and dictionary support.
* [ ] Add buildin functions.
