//Horrace, Robert
/*
	This program is for HW3.
	This is the first prod/cons
	program for the assignment.
*/

use circQueue;

config const numItems: int = 32;

var notempty$: sync bool = false;
var notfull$: sync bool = true;

var itemsprod: int = 0;
var itemsrem: int = 0;

proc produce(){
	var id: int;
	for i in 1..numItems do{
		while(notfull$ == false){}
		id = add(i);
		writeln("Producer added item ",i," added at buf[",id,"]");
		itemsprod += 1;
		notempty$ = true;
		if(tail$ == head$){
			notfull$ = false;
		}
	}
}

proc consume(){
	var id, item:int;
	for i in 1..numItems do{
		while(notempty$ == false){}
		var pair = remove();
		(id,item) = pair;
		writeln("Consumer removed item ",item," from buf[",id,"]");
		itemsrem += 1;
		notfull$ = true;
		if(head$ == tail$){
			notempty$ = false;
		}
	}
}

sync{
	begin produce();
	begin consume();
}
writeln("Items produced: ",itemsprod);
writeln("Items removed: ",itemsrem);
