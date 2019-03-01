// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _steamGen_H_
#define _steamGen_H_
//add your includes for the project steamGen here


//end of add your includes here
#ifdef DEBUG
	#define debug(x)	Serial.println(x);
	#define debug_c(x)	Serial.print(x);
#else
	#define debug(x)	;
	#define debug_c(x)	;
#endif


//add your function definitions for the project steamGen here




//Do not add code below this line
#endif /* _steamGen_H_ */
