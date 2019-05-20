//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A circular queue (suitable for concurrent accesses)
//

module circQueue {

  config const bufSize = 10;		// buffer size (configurable)

  var buf$: [0..bufSize-1] sync int;	// buffer (cells are self-sync)
  var head$ = 0;  	                // head idx 
  var tail$ = 0;		        // tail idx 

  // add a new item at tail
  // return idx of position
  proc add(item) {
    var idx = tail$;
    tail$ = (idx + 1) % bufSize;
    buf$[idx] = item;
    return idx;
  }

  // remove an item from head
  // return (idx, item) pair
  proc remove() {
    var idx = head$;
    head$ = (idx + 1) % bufSize;
    var item = buf$[idx];
    var pair = (idx, item);
    return pair;
  }

}

