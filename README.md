# Function pack Autonomous Driving

<br>
This is just a template. <br> 
<br> 
<br>
Note: <br> 
- There are no default colours. <br> 
- The directions might not be exact. Moreover, North might not be in the direction from which you start. <br>
<br> 
<br>
TEST <br>
The test folder is there to test new ideas and etc. <br> 
They definitely do not work, because they are tests, not prototype releases. <br>
And they are not `.c` files but `.cpp` files. <br>
<br> 
<br> 
Releases <br> 
To use releases, you download the release file, then copy the contents into your project c file. <br> 
Normally, it is pasted just above function game0(). <br> 
If function game0() is not present, create it just above line: 'DLL_EXPORT void onTimer()'. You should also read through the function 'onTimer()' and ensure that Game0() is called under 'case 0:' in operator 'switch (CurGame)'. <br>
