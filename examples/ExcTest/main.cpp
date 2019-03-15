//
//  main.cpp
//  ExcTest
//
//  Created by WangDonghui on 2019/3/14.
//  Copyright © 2019年 WangDonghui. All rights reserved.
//

#include <iostream>

#include "../../signal_recovery.h"

void excFunc() {
    int* ptr = NULL;
    *ptr = 0;
}

int main() {
    signal_catch_init();

    signal_try(label0) {
        signal_try(1) {
            int a = 3;
            int b = 0;
            int c = a/b;
        }
        signal_catch(1) {
            std::cout << "catched signal " << signal_name(signal_info()->si_signo) << std::endl;
        }
        signal_end(1)

        excFunc();
    }
    signal_catch(label0) {
        std::cout << "catched signal " << signal_name(signal_info()->si_signo) << std::endl;
    }
    signal_end(label0)

    std::cout << "Hello, World!" << std::endl;
    return 0;
}