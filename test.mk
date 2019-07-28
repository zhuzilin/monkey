let fib = fn(n) {
	if(n < 3) { n }
	else { fib(n-1) + fib(n-2) }
}

fib(30)