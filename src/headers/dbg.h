// MACROS : 
/*
-> A macro is a name given to a peice of code which defined by a #define directive whenever a macro is referenced in the program the compiler simply replaces the name of the macro with its value
 
SYNTAX :
-> Syntax of a macro : #define name value
-> A macro like all other things is not to be terminated by a semi-color(;)

TYPES : 
-> There are mainly two types of macros : 1. Object like 2. Function like
1. Object like : This is somewhat like a constant that you define. For eg : #define PI 3.14
   -> Now wherever PI will be used it will be replaced with 3.14
2. Function like : This is defined as a block which contains a peice of code to be performed
   -> You can use jump statements, call other functions, declare variables and all other things with it
   -> For eg : add(a,b) (a+b), then int c = add(a,b), the add(a,b) is simply replaced by (a+b) so in actuality
   -> it is equivalent to the expression : int c = (a+b)
   -> Now remember for a complex statement make sure to enclose it some brackets({} or ()) 	
*/

#ifndef __dbg_h__ // This is a pre-processor directive to protect the header from being included multiple times
#define __dbg_h__ // It says if not defined __dbg_h__ then defined it or else skip that whole block

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef NDEBUG // This statement will let us turn on/off the debugging messages as per our need 
// This is simply means that if you have already defined the NDEBUG variable then the given statement will be executed
#define debug(M, ...) // Here in case of NDEBUG the debug macro is empty and has no value (Check the makefile to gain info about how to define vairables while compiling)
#else
// However if you have not defined NDEBUG then the program is to be ran with debug messages and we define the debug macro properly
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",__FILE__, __LINE__, ##__VA_ARGS__)
#endif

// This macro makes use of a ternary operator(statement ? True : False)
// It basically checks the value of the errno and if its 0, then value of this macro will become "None" else the value of the macro will be a string defining the type of error
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// This macro is used for simple logging a message to the error stream
// As discussed earlier you can very much call other functions in the definition of the macro
// So here we are simply calling the fprintf() function with a specific format : Syntax : fprintf(output-stream,format,arguments)
// __FILE__ : This is pre-defined string which contains the name of the file where it is called
// __LINE__ : This is a pre-defined variable which stores the line number where its called is used
// ##__VA_ARGS__ : This is used to store the variable argument provided by the user replacing the '...' part of the argument and all the contents of the '...' are stored in the variable defined just in front of it
// In this case the the variable M stores all of the data provided in __VA_ARGS__
#define log_err(M, ...) fprintf(stderr,"[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,\
						clean_errno(), ##__VA_ARGS__)

// This macro is exactly the same as the macro above but has a different use case as we will use this to log warnings
#define log_warn(M, ...) fprintf(stderr,"[WARN] (%s:%d: errno: %s) " M "\n",__FILE__, __LINE__,\
						clean_errno(), ##__VA_ARGS__)

// This macro is exactly the same as the macro above but has a different use case as we will use this to log information
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n",__FILE__, __LINE__, ##__VA_ARGS__)

// Now the general case of defining a function is : 
// 1. Do some work 2. Check for errors 3. Cleanup the resources
// Now the below macro does exactly that it will first check the value of the provided variable A
// and its checks it like the assert function (For eg : if I wrote assert(A == 1), so it will check if A is infact 1 and if not then terminate the program)
// However instead of terminating the program it will be first hard reset the errno variable as its convention and then jump to the cleanup section of the program
#define check(A, M, ...) if(!(A)){ log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

// This works exactly the same the macro above without any error checking however its use is different
// As this macro will be placed in that region of the program which is not supposed to run (like the default case of a switch)
// and just like the macro it also does the error cleanup
#define sentinel(M, ...) {log_err(M, ##__VA_ARGS__);errno = 0; goto error; }

// This macro calls another macro check with a specific message suggesting that there is a memory error
#define check_mem(A) check((A),"Out of memory.")

// This macro define the previous check macor however for logging error it uses the debug macro
// So in case we compile the program in release mode, it will still do the cleanup but won't log any messages to the console
#define check_debug(A, M, ...) if((!A)) {debug(M, ##__VA_ARGS__); errno = 0; goto error; }

#endif

// Using the character '\' is optional as it works as an escape character, which lets us jump to the next line
// while still continuing the flow of the program
// Eg : You can define a statement to be printed like this
// 1. printf("Hello there");
// 2. printf("Hello\
			there");
// Both ways are valid 
// Using '\' character we can still continue writing our statement even when its in new line
