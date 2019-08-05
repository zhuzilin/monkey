# monkey
A C++ version [monkey](https://monkeylang.org/) language interpreter. From [Write An Interpreter In Go](https://interpreterbook.com/).
With additional mark-and-sweep garbage collection.
## Usage
You can use the vscode config to build and run the interpreter. Or
```bash
> g++ -std=c++11 main.cpp src/*.cpp -o monkey --debug
> ./monkey test.mk
hello world!
minimal prime factor for 1321231 is 487 

return: 
type:  NULL
value: NULL
```
The `test.mk` is function to get minimal prime factor.
```js
print("hello world!");

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
	print(a, "is a prime!");
} else {
	print("minimal prime factor for", a, "is", f);
}
```
And `repl.cpp` is the REPL(Read-Eval-Print Loop) main function, to only use parser or lexer, you can change to `rppl.cpp` or `rlpl.cpp`.

## TODOs
* [x] Add garbage collection.
* [x] Add array support.
* [x] Add buildin functions.
* [ ] Add dictionary support.
* [ ] Add error handling for lexer.

