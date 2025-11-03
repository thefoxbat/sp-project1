/*
 * CS 551 Project "Memory manager".
 * You do not need to turn in this file.	
 */

#include "memory_manager.h"

int main(int argc, char * argv[])
{
	// Your code
    printf("===========Memory Manager Test===========\n");

    printf("Test 1 : Initialize memory manager... \n");
    mem_mngr_init();
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 2 : Allocate 5 bytes (first slot)... \n");
    void *ptr1 = mem_mngr_alloc(5);
    printf("Allocated address: %p\n", ptr1); 
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 3 : Allocate 6 bytes (second slot)... \n");
    void *ptr2 = mem_mngr_alloc(6);
    printf("Allocated at address: %p\n", ptr2);
    mem_mngr_print_snapshot(); 
    printf("\n");

    printf("Test 4 : Allocate 6 more slots... \n");

    void *ptr3 = mem_mngr_alloc(8);
    void *ptr4 = mem_mngr_alloc(7);
    void *ptr5 = mem_mngr_alloc(3);
    void *ptr6 = mem_mngr_alloc(8);
    void *ptr7 = mem_mngr_alloc(2);
    void *ptr8 = mem_mngr_alloc(8);
    printf("All 8 slots in first batch allocated.\n");
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 5: Allocating 9th slot (7 bytes) - trigger new batch... \n");
    void *ptr9 = mem_mngr_alloc(7);
    printf("Allocated at address: %p\n", ptr9);
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 6: Freeing ptr3 (slot in the middle)... \n");
    mem_mngr_free(ptr3);
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 7: Allocating 4 bytes (should reuse freed slot)... \n");
    void *ptr10 = mem_mngr_alloc(4);
    printf("Allocated at address: %p (comapre with ptr3: %p)\n", ptr10, ptr3);
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 8: Testing double free error... \n");
    mem_mngr_free(ptr3); 
    printf("\n");

    printf("Test 9: Testing invalid free error... \n");
    int dummy = 42;
    mem_mngr_free(&dummy);
    printf("\n");

    printf("Test 10: Testing misaligned pointer error... \n");
    char *misaligned = (char *)ptr1 + 3;
    mem_mngr_free(misaligned);
    printf("\n");

    printf("Test 11: Freeing all the allocated slots... \n");
    mem_mngr_free(ptr1);
    mem_mngr_free(ptr2);
    mem_mngr_free(ptr4);
    mem_mngr_free(ptr5);
    mem_mngr_free(ptr6);
    mem_mngr_free(ptr7);
    mem_mngr_free(ptr8);
    mem_mngr_free(ptr9);
    mem_mngr_free(ptr10);
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 12: Allocating after freeing all (should use frist slot)... \n");
    void *ptr11 = mem_mngr_alloc(5);
    printf("Allocated at address: %p (comapre with ptr1: %p)\n", ptr11, ptr1);
    mem_mngr_print_snapshot();
    printf("\n");

    printf("Test 13: Cleaning up memory manager... \n");
    mem_mngr_leave();
    printf("Memory manager cleaned up successfully.\n\n");

    printf("===========All Test Completed===========\n");

    return 0;
}
