# monkey
A C++ version [monkey](https://monkeylang.org/) language interpreter. From [Write An Interpreter In Go](https://interpreterbook.com/).
With additional mark-and-sweep garbage control.
## Usage
You can use the vscode config to build and run the interpreter. Or
```bash
> g++ -std=c++11 main.cpp src/*.cpp -o monkey --debug
> ./monkey test.mk
> Integer
> 487
```
The `test.mk` is function to get minimal prime factor.
```js
let a = 1321231;

// find mininum factor that is greater than 1
let minFactor = fn (a) {
	let i = 2;
	while(true) {
		if (a % i == 0) { 
			return i;
		}
		let i = i + 1;
	}
}

let f = minFactor(a);

if (f == a) {
	return "prime!";
} else {
	return f;
}
```
And `repl.cpp` is the REPL(Read-Eval-Print Loop) main function, to only use parser or lexer, you can change to `rppl.cpp` or `rlpl.cpp`.

## TODOs
* [ ] Add error handling for lexer.
* [x] Add garbage collection.
* [ ] Add array and dictionary support.
* [ ] Add buildin functions.
