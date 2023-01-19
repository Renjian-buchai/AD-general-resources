# Function pack Autonomous Driving

<br>
This is just a template. <br> 
The pragma regions are there for the convenience of fellow VS code users. (VS sadly get the benefits, too.) <br>
// pragma was deleted because I opened it up in notepad and it made the code much harder to read instead. 
<br> 
<br>
Note: <br> 
- There are no default colours. <br> 
- The directions might not be exact. Moreover, North might not be in the direction from which you start. <br>
- The `gameStateFunctions.c` functions and `functions.c` functions are **not interchangeable**. The only one that can be used is the lineFollow functions. <br>
<br> 
<br>
How to control the flow: <br> 
gameState system <br>
- Copy `gameStateFunctions.c` into the `.c` file created by the software, outside of game0(). For standardisation, paste it just above game0(). <br> 
- Create gameState object, type int. <br> 
- Code in game0() follows format: <br> 
&ensp;- `if( gameState == [integer] ){ [Action]; if( is[condition] ){ stateUp; } }` <br> 
&ensp;Alternative: `if( gameState == [integer] ){ if( not[Condition] ){ [Action]; } stateUp; } // to use this, copy from `notFunctions.c` instead of `Functions.c`<br>
&ensp;Note: if possible, only include **one** action. <br>
&ensp;Further note: the code need not be in chronological order. <br>
<br> 
<br>
// ! Doesn't work because game0() is called 40 times per second.<br> 
while loops <br> 
- Copy `functions.c` into the `.c` file created by the software, outside of game0(). For standardisation, paste it just above game0(). <br>
- Code in game0() follows format: <br> 
&ensp;- `while( not[condition] ){ [action] }` <Br>
&ensp;Note: The code must be in chronological order. <br>
<br> 
<br> 
TEST <br>
The test folder is there to test new ideas and etc. <br> 
They definitely do not work, else they wouldn't be there. <br>
And they are not `.c` files but `.cpp` files. <br>
