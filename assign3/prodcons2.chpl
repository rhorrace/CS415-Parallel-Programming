//Horrace, Robert
/*
	This program is for HW3.
	This is the first prod/cons
	program for the assignment.
*/

use circQueue;

config const numItems: int = 32;
config const numProds: int = 2;
config const numCons: int = 2;

var notempty$: sync bool = false;
var notfull$: sync bool = true;
var lastitem$: sync bool = false;;
var itemsprod$: int = 0;
var itemsrem$: int = 0;

proc produce(tid){
	var n = numItems/numProds;
	for i in 1..n do{
		while(notfull$ == false){}
		var id = add(i);
		writeln("Producer[",tid,"] added item ",i," added at buf[",id,"]");
		notempty$ = true;
		itemsprod$ += 1;
		if(tail$ == head$){
			notfull$ = false;
		}
	}
}

proc consume(tid){
	var n: int = numItems/numCons;
	var id, item:int;
	for i in 1..n do{
		while(notempty$ == false){}
		var pair = remove();
		(id,item) = pair;
		writeln("Consumer[",tid,"] removed item ",item," from buf[",id,"]");
		itemsrem$ += 1;
		notfull$ = true;
		if(head$ == tail$){
			notempty$ = false;
		}
	}
}

if(numItems%numProds != 0 || numItems%numCons != 0){
	writeln("Error: not a valid amount of producers/consumers");
}
else{
	sync{
		forall i in 1..numProds do{
			begin produce(i);
		}
		forall i in 1..numCons do{
			begin consume(i);
		}
	}
	writeln("Items prodruced: ",itemsprod$);
	writeln("Items removed: ",itemsrem$);
}
