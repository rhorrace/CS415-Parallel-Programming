
config const N = 8;

var D: domain(2) = {0..N-1,0..N-1};

var a:[D] int = [(i,j) in D] i+j;
var b:[D] int = 1;
var c:[D] int = 0;
var totalsum: int = 0;


forall (i,j) in c.domain do {
		c[i,j] += +reduce(a[i,0..N-1] * b[0..N-1,j]);
}

totalsum = +reduce c;
writeln("Total sum of Matrix C = ", totalsum ," (N=8: 3584,N=16: 61440,N=32: 1015808,N=64: 16515072)");


