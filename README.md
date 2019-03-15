# Signal Recovery
## Functional introduction:
This library enables programs to recover from operating system exceptions such as EXC_BAD_ACCESS and EXC_ARITHMETIC. Use try - catch exception handling syntax similar to programming language. If an operating system exception occurs in the signal_try block, the program jumps to the signal_catch block to continue execution. If the code is carefully organized, there will be no abnormalities in the program from the end-user's point of view, and the function of the program will not be affected.
## Notes:
* Recovery from operating system exceptions masks some errors. Operating system exceptions indicate that there is a serious BUG in the program. The first thing the programmer should do is to find out the root cause of the BUG and fix it, rather than hide it.
* Recovery from operating system exceptions is bound to be accompanied by stack unwinding, which skips some code that should normally be executed, so there may be memory leaks and resource leaks.
* Preparations for recovery from operating system exceptions (even if they do not occur) require system resources (CPU, memory, etc.), and code segments that are frequently invoked should be used with caution.
## Applicable scenarios:
* Deal with occasional collapses that are extremely difficult to recur.
* There are occasional crashes in third-party libraries, and the authors of third-party libraries are unable to repair BUG in time.
* For quick fixes to emergency crashes, later versions should be thoroughly repaired.
## Functional characteristics:
* Thread Safety
* Support nesting
* Multilingual Support (OC/C/C++)
* Multi-Platform Support (iOS/macOS/Linux)
## Method of use:
```
    // Global init
    signal_try_catch_init();

    // Three labels must be the same, can be empty.
    // Nested try-catch pairs must use different label names.
    signal_try(label) {
        // Add your code need try
    }
    signal_catch(label) {
        // Add your code to process exceptions, or do nothing.
        siginfo_t* info = signal_info();
    }
    signal_end(label)
```
## Instructions:
* Different try-catch blocks (including nested try-catch blocks) in the same scope can not have the same label name.
* Do not forget global initialization. Global initialization only needs to be run once. 
* If you need to deal with more signal types, you can modify the code in the signal_handler_init() function.
* Debugging example in IDEs such as XCode, CLion, etc. will show the code where the exception occurs and cannot continue to execute. This is because the debugger can take precedence over the application in handling exceptions and preventing the program from continuing to execute. The correct way is to run the example separately without debugging.
## Realization principle:
The process of program crash is as follows: the operating system detects runtime errors such as illegal memory access or illegal instructions, and then sends out a corresponding POSIX signal. If the program is not processed, it calls the default processing function, usually pops up a prompt dialog box, and ends the process. The application can process the signal first, but without some preparation, the crash is irreversible. To prevent program crash, we need to register signal handler beforehand, and then save a complete register state with sigsetjmp in signal_try() phase. When signal is generated, we can use siglongjmp to restore the register state saved before then the program can continue to execute.
## References:
* [https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sigsetjmp.3.html](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sigsetjmp.3.html)
* [https://linux.die.net/man/3/sigsetjmp](https://linux.die.net/man/3/sigsetjmp)
* [https://linux.die.net/man/3/longjmp](https://linux.die.net/man/3/longjmp)
* [https://lackingrhoticity.blogspot.com/2013/08/handling-crashes-on-mac-os-x.html](https://lackingrhoticity.blogspot.com/2013/08/handling-crashes-on-mac-os-x.html)
