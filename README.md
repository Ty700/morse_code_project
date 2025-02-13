### Description:

This project involves creating a Morse code translator in C that controls an LED on the Milk-V Duo to blink Morse code representations of text inputs. The objective is to blend software development with hands-on hardware interaction, focusing on embedded systems programming, C language, and unit testing using Unity.

### How to build a binary

```
USAGE:

 ./build.py -<ARGUMENT>

   Example:
       ./build.py -p <= Builds a binary that will run on host machine without debug statements nor runs unit tests
       ./build.py -c <= Will remove the binary (and cmake build) created by ./build.py -h

    Important Notes:
        - Only one argument is allowed.
        - Building for host also runs the binary.
        - You can find the built binary in /bin/
    
    Help:
        ./build.py -h | --help

 ARGUEMENTS:

    CLEANING:

       -c  <= "Clean"
           Feature:
               [X] Removes all binaries
               [X] Removes build directory
 
    BUILDING FOR HOST:

       Features for HOST options:
            [X] x86 binary       
       
       -p  <= "Production"
           Features: 
                [ ] Unit Tests
                [ ] Debug Statements
            - Honestly only use this just to see if it will compile w/out error.
            - Running this has no output since the output is blinking an LED
            - Stuff is happening... ./build.py -d to see what is going on behind the scenes

       -d  <= "Debug"
           Features: 
                [ ] Unit Tests
                [X] Debug Statements

       -u <= "Unit testing"
           Features: 
                [X] Unit Tests
                [ ] Debug Statements

       -v   <= "Verbose Unit test"
           Features: 
                [X] Unit Tests
                [X] Debug Statements
   
    BUILDING FOR TARGET:
        
        Notes: 
            - The text char array in src/main.c will be the message that is blinked out in Morse code.
            - Can not run unit tests on target for obvious reasons. Those are mainly to test morse code logic.

        -t <= "Target"
            Features:
                [ ] A cookie (Ran out)
                [X] A working RISC-V binary for MILK
            
        -td <= "Target Debug"
            Features:
                [X] A working RISC-V binary for MILK 
                [X] Debug statements 
```

