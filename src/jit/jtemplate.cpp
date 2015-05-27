#include "jtemplate.h"

using namespace JIT;

uint8_t Template::prologue[] = {
    0x55, // push rbp (backup RBP)
    0x48, 0x89, 0xE5, // mov rsp, rbp (backup RSP)
};

uint8_t Template::epilogue[] = {
    0x48, 0x89, 0xEC //mov rbp, rsp (restore RSP)
    0x5d, // pop rbp (restore RBP)
    0xC3 // ret
};
