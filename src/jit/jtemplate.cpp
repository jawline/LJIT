#include "jtemplate.h"

using namespace JIT;

uint8_t Template::prologue[] = {
    0x55, // push rbp
    0x48, 0x89, 0xE5, // mov rsp, rbp
    // backup callee saved registers
    0x41, 0x54, // pushq %r12
    0x41, 0x55, // pushq %r13
    0x41, 0x56, // pushq %r14
    // %rdi = memset
    // %rsi = putchar
    // %rdx = getgetchar
    // backup args to callee saved registers
    0x49, 0x89, 0xFC, // movq %rdi, %r12
    0x49, 0x89, 0xF5, // movq %rsi, %r13
    0x49, 0x89, 0xD6, // movq %rdx, %r14
    // %r12 = memset
    // %r13 = putchar
    // %r14 = getchar
    // allocate 30,008 B on stack
    0x48, 0x81, 0xEC, 0x38, 0x75, 0x00, 0x00, // subq $30000, %rsp
    // address of beginning of tape
    0x48, 0x8D, 0x3C, 0x24, // leaq (%rsp), %rdi
    // fill with 0's
    0xBE, 0x00, 0x00, 0x00, 0x00, // movl $0, %esi
    // length 30,000 B
    0x48, 0xC7, 0xC2, 0x30, 0x75, 0x00, 0x00, // movq $30000, %rdx
    // memset
    0x41, 0xFF, 0xD4, // callq *%r12
    0x49, 0x89, 0xE4 // movq %rsp, %r12
    // %r12 = &tape[0];
};

uint8_t Template::epilogue[] = {
	0x48, 0x81, 0xC4, 0x38, 0x75, 0x00, 0x00, // addq $30008, %rsp
    // restore callee saved registers
    0x41, 0x5E, // popq %r14
    0x41, 0x5D, // popq %r13
    0x41, 0x5C, // popq %r12
    0x5d, // pop rbp
    0xC3 // ret
};