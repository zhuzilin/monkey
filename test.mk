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

let fib = fn (n) {
	if (n < 3) { return 1; }
	else { return fib(n-1) + fib(n-2); }
}

// fib(30);

let id = fn(n) {
	if (!n) { return n; }
	return id(n-1) + 1;
}

id(1000);