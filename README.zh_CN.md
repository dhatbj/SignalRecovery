# SignalRecovery
## 功能介绍：
这个库可以使程序从 EXC_BAD_ACCESS、EXC_ARITHMETIC 等操作系统异常中恢复。使用方法类似编程语言的 try - catch 异常处理语法。如果 signal_try 块中发生操作系统异常，程序会跳到 signal_catch 块继续执行。如果精心组织代码，从最终用户角度完全感觉不到程序内部有异常发生，程序功能也不会受到影响。

## 注意事项：
* 从操作系统异常中恢复的行为掩盖了某些错误。操作系统异常标志着程序中有严重BUG, 程序作者首先要做的是找出BUG的根源并修复它，而不是掩盖它。
* 从操作系统异常中恢复必然伴随着栈展开(stack unwinding)，跳过了一部分正常情况下应该执行的代码，所以有可能发生内存泄漏和资源泄漏。
* 为了从操作系统异常中恢复所做的准备工作（即使异常并没有发生）也是要占用系统资源的（CPU、内存等），对于频繁调用的代码段应谨慎使用。

## 适用场景：
* 处理极难复现的偶发性崩溃。
* 第三方库中有偶发的崩溃, 而第三方库的作者又无法及时修复BUG。
* 用于快速修复紧急的崩溃问题，之后的版本还是要彻底修复为好。

## 功能特点：
* 线程安全
* 支持嵌套
* 多语言支持（OC/C/C++）
* 多平台支持（iOS/macOS/Linux）

## 使用方法：
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
}
signal_end(label)
```
## 注意事项：
* 同一作用域里不同的 try-catch 块（包括嵌套使用的 try-catch 块），label 名不能相同。
* 一定不要忘了全局初始化。全局初始化不分线程，一个进程初始化一次即可。
* 如果需要处理更多的 signal 类型，可以在 signal_handler_init() 函数里修改代码添加。
* 在 XCode 中运行程序，XCode会停在发生异常的代码处无法继续执行。这是因为 XCode 调试器可以比程序代码更优先处理异常，并且不让程序继续执行。

## 实现原理：
程序崩溃的过程是这样的：操作系统检测到内存非法访问、非法指令等运行时错误，然后发出一个对应的 POSIX signal，如果程序没有处理，就调用默认处理函数，一般是弹出一个提示对话框，然后结束进程。应用程序可以先一步处理 signal, 但如果没有事先做一些准备工作，崩溃已无法挽回了。要防止程序崩溃，需要事先注册 signal handler，然后在 signal_try() 阶段用 sigsetjmp 保存一份完整的寄存器状态，当 signal 产生时，使用 siglongjmp 恢复之前保存的寄存器状态，程序就可以继续执行了。

## 参考资料：
* [https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sigsetjmp.3.html](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sigsetjmp.3.html)
* [https://linux.die.net/man/3/sigsetjmp](https://linux.die.net/man/3/sigsetjmp)
* [https://linux.die.net/man/3/longjmp](https://linux.die.net/man/3/longjmp)
* [https://lackingrhoticity.blogspot.com/2013/08/handling-crashes-on-mac-os-x.html](https://lackingrhoticity.blogspot.com/2013/08/handling-crashes-on-mac-os-x.html)
