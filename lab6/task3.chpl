// Hello World (task version 3).
//

config const numTasks = here.numPUs(true,true);
coforall tid in 0..#numTasks do
  writeln("Hello, world! (from task " + tid + " of " + numTasks + ")");
