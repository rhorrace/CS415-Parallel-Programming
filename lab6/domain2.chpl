// More domain and array examples.
//

var D = {1..6} by 2;         // declare a domain D
var a: [D] int = [i in D] i; // declare an array over D

writeln("D = ", D, "\t// a striding domain");
writeln("a = ", a, "\t// a is defined over D; a has ", a.numElements, " elements\n");

// redefine domain D, what happens to a?
D = {1..8} by 2;
writeln("D = ", D, "\t// redefine D to a superset of original D");
writeln("a = ", a, "\t// a has ", a.numElements, " elements now\n");

// redefine domain D, what happens to a?
D = {1..8} by 1;
writeln("D = ", D, "\t// redefine D to another superset");
writeln("a = ", a, "\t// a has ", a.numElements, " elements now\n");

// redefine domain D, what happens to a?
D = {2..7} by 2;
writeln("D = ", D, "\t// one more, a disjoint domain");
writeln("a = ", a, "\t// a has ", a.numElements, " elements now");

