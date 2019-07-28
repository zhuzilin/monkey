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