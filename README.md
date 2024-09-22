<h1>Notes, examples, and Questions about C++ Concurrency In Action Practical Multithreading</h1>



Refer to "notes.txt" <br>
Refer to "Questions.txt" <br>
<br>
Dont pass pointers, references to protected data outside the scope of the lock.
Whether by returning them from a function, storing them in externally visible memory. Or passing them as arguments to a user supplied functions.
<br>


ch 3 focuses on the dangers that can happen when sharing data b/w threads<br>
ch 4 uses conditional variable to have threads synchronize through notifications <br>