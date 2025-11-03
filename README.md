**CS 551 Programming Project 1: Custom Memory Manager**

**Name:** Aditya Prakash

**B-Number:** B01157609

**Project Status**

All project requirements are fully implemented, tested, and passed.

**Implementation Overview**

This project implements a custom memory manager designed to efficiently handle heap memory allocation and deallocation at the program level. The implementation relies on two main components: **Bitmap Operations** and the **Memory Manager Logic**.

**1\. Bitmap Operations (bitmap.c)**

This component provides the low-level functions required to manage the allocation status of memory slots.

| **Function** | **Description** |
| --- | --- |
| bitmap_find_fisrt_bit() | Finds the position of the first bit with a specified value (0 or 1). |
| bitmap_set_bit() | Sets a specific bit to 1 (Allocated). |
| bitmap_clear_bit() | Clears a specific bit to 0 (Free). |
| bitmap_bit_is_set() | Checks if a specific bit is set to 1. |
| bitmap_print_bitmap() | Prints the contents of the bitmap (provided implementation). |

**Key Implementation Details:**

- Uses **bitwise operations** for efficient and fast manipulation of bits.
- Includes **comprehensive error checking** for NULL pointers and boundary conditions.

**2\. Memory Manager Logic (memory_manager.c)**

This component manages the memory lists, batches, and implements the core allocation/deallocation strategies.

| **Function** | **Description** |
| --- | --- |
| mem_mngr_init() | Initializes the memory manager with a default 8-byte slot list and one batch of slots. |
| mem_mngr_alloc() | Allocates memory slots using the **first-fit policy**. |
| mem_mngr_free() | Frees allocated memory with **comprehensive error detection**. |
| mem_mngr_leave() | Cleans up and frees all dynamically allocated memory before program termination. |

**Memory Allocation Strategy**

- **Pre-allocation:** Memory is pre-allocated in batches to minimize kernel/user space switches.
- **Tracking:** A **dynamically sized bitmap** tracks the status of all slots (1 = free, 0 = allocated).
- **Policy:** Implements a **first-fit policy**, returning the slot with the smallest address.
- **Expansion:** Automatically expands the bitmap and dynamically allocates new batches when all current slots are occupied.

**Error Detection (mem_mngr_free())**

The mem_mngr_free() function is designed to detect three types of critical errors:

- **Double Freeing:** Attempting to free an already-freed slot.
- **Invalid Pointer:** Providing a pointer that is outside the managed memory pool.
- **Misaligned Pointer:** Providing a pointer that is not located exactly at the start of a memory slot.

**Building and Testing the Project**

The project uses a Makefile for compilation.

**Build Commands (in Bash or MinGW)**

| **Action** | **Command** |
| --- | --- |
| **Clean Project** | mingw32-make clean |
| **Build Executable** | mingw32-make all or mingw32-make test |
| **Run Program** | ./test_main |

**Testing Overview**

The implementation has been thoroughly tested with the provided test program across the following scenarios:

- Multiple allocation and deallocation cycles.
- Batch expansion scenarios, verifying correct bitmap resizing.
- All three error conditions in mem_mngr_free() (Double Free, Invalid Pointer, Misaligned Pointer).
- Testing with different MEM_ALIGNMENT_BOUNDARY and MEM_BATCH_SLOT_COUNT values.

**Design Decisions**

- **First-Fit Policy:** The policy is naturally implemented by leveraging the bitmap_find_first_bit() function, which returns the lowest available index/address.
- **Bitmap Expansion:** When adding new batches, the bitmap is expanded using a realloc-style approach (allocate new, copy old, free old) to preserve existing allocation data.
- **Memory Organization:** A **single memory list with 8-byte slots** is used, adhering to the project assumption that all allocation requests are \$\\le 8\$ bytes.
- **Error Handling:** Comprehensive error checking provides descriptive error messages to aid in debugging.

**Files Submitted**

- bitmap.c - Bitmap operations implementation
- memory_manager.c - Memory manager implementation
- memory_manager.h - Memory Manager header (unchanged)
- common.h - Common definitions (unchanged)
- Makefile - Build configuration
- README.md - This file

**Notes**

- The implementation assumes all allocation requests are \$\\le 8\$ bytes (as stated in the project requirements).
- The code is designed to work with any MEM_ALIGNMENT_BOUNDARY and MEM_BATCH_SLOT_COUNT values that are multiples of 8.
- All dynamically allocated memory is properly freed in mem_mngr_leave().
- The implementation uses efficient bitwise operations for bitmap manipulation.