#include "jtemplate.h"

using namespace JIT;

uint8_t Template::prologue[] = {
    0x55, // push rbp (backup RBP)
    0x41, 0x54, //push r12
    0x49, 0x89, 0xFC, //mov rdi, r12
    0x48, 0x89, 0xE5 // mov rsp, rbp (backup RSP)
};

uint8_t Template::epilogue[] = {
    0x58, //pop rax (placing the result of our lovely math into RAX)
    0x48, 0x89, 0xEC, //mov rbp, rsp (restore RSP)
    0x41, 0x5C, //pop r12
    0x5d, // pop rbp (restore RBP)
    0xC3 // ret
};

size_t Template::prologueSize() {
	return sizeof(prologue);
}

size_t Template::epilogueSize() {
	return sizeof(epilogue);
}
