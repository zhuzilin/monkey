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