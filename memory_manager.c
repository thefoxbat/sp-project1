/*
 * CS 551 Project "Memory manager".
 * This file needs to be turned in.	
 */


#include "memory_manager.h"

static STRU_MEM_LIST * mem_pool = NULL;

/*
 * Print out the current status of the memory manager.
 * Reading this function may help you understand how the memory manager organizes the memory.
 * Do not change the implementation of this function. It will be used to help the grading.
 */
void mem_mngr_print_snapshot(void)
{
    STRU_MEM_LIST * mem_list = NULL;

    printf("============== Memory snapshot ===============\n");

    mem_list = mem_pool; // Get the first memory list
    while(NULL != mem_list)
    {
        STRU_MEM_BATCH * mem_batch = mem_list->first_batch; // Get the first mem batch from the list 

        printf("mem_list %p slot_size %d batch_count %d free_slot_bitmap %p\n", 
                   mem_list, mem_list->slot_size, mem_list->batch_count, mem_list->free_slots_bitmap);
        bitmap_print_bitmap(mem_list->free_slots_bitmap, mem_list->bitmap_size);

        while (NULL != mem_batch)
        {
            printf("\t mem_batch %p batch_mem %p\n", mem_batch, mem_batch->batch_mem);
            mem_batch = mem_batch->next_batch; // get next mem batch
        }

        mem_list = mem_list->next_list;
    }

    printf("==============================================\n");
}

/*
 * Initialize the memory manager with 8 bytes slot size mem_list.
 * Initialize this list with 1 batch of slots.
 */
void mem_mngr_init(void)
{
    
    mem_pool = (STRU_MEM_LIST *)malloc(sizeof(STRU_MEM_LIST));
    if (mem_pool == NULL)
    {
        printf("ERROR: Failed to allocate memory for mem_pool\n");
        return;
    }

    mem_pool->slot_size = MEM_ALIGNMENT_BOUNDARY;
    mem_pool->batch_count = 1;
    mem_pool->next_list = NULL;

    mem_pool->bitmap_size = (MEM_BATCH_SLOT_COUNT + BIT_PER_BYTE - 1) / BIT_PER_BYTE;

    mem_pool->free_slots_bitmap = (unsigned char *)malloc(mem_pool->bitmap_size);
    if(NULL == mem_pool->free_slots_bitmap)
    {
        printf("ERROR: Failed to allocate memory for free_slots_bitmap\n");
        free(mem_pool);
        mem_pool = NULL;
        return;
    }

    memset(mem_pool->free_slots_bitmap, 0xFF, mem_pool->bitmap_size);

    mem_pool->first_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
    if(NULL == mem_pool->first_batch)
    {
        printf("ERROR: Failed to allocate memory for first_batch\n");
        free(mem_pool->free_slots_bitmap);
        free(mem_pool);
        mem_pool = NULL;
        return;
    }

    mem_pool->first_batch->batch_mem = malloc(mem_pool->slot_size * MEM_BATCH_SLOT_COUNT);
    if(NULL == mem_pool->first_batch->batch_mem)
    {
        printf("ERROR: Failed to allocate batch memory\n");
        free(mem_pool->first_batch);
        free(mem_pool->free_slots_bitmap);
        free(mem_pool);
        mem_pool = NULL;
        return;
    }

    mem_pool->first_batch->next_batch = NULL;
}

/*
 * Clean up the memory manager (e.g., release all the memory allocated)
 */
void mem_mngr_leave(void)
{
    STRU_MEM_LIST * current_list = mem_pool;

    while(NULL != current_list)
    {
        STRU_MEM_LIST * next_list = current_list->next_list;

        STRU_MEM_BATCH * current_batch = current_list->first_batch;
        while(NULL != current_batch)
        {
            STRU_MEM_BATCH * next_batch = current_batch->next_batch;
            
            if(NULL != current_batch->batch_mem)
            {
                free(current_batch->batch_mem);
            }

            free(current_batch);

            current_batch = next_batch;
        }

        if(NULL != current_list->free_slots_bitmap)
        {
            free(current_list->free_slots_bitmap);
        }

        free(current_list);

        current_list = next_list;
    }
    
    mem_pool = NULL;
}

/*
 * Allocate a chunk of memory 	
 * @param size: size in bytes to be allocated
 * @return: the pointer to the allocated memory slot
 */
void * mem_mngr_alloc(size_t size)
{
    if(NULL == mem_pool)
    {
        printf("ERROR: Memory manager not initialized\n");
        return NULL;
    }

    if(size == 0)
    {
        return NULL;
    }

    int aligned_size = SLOT_ALLINED_SIZE(size);

    STRU_MEM_LIST * mem_list = mem_pool;
    while (NULL != mem_list && mem_list->slot_size != aligned_size)
    {
        mem_list = mem_list->next_list;
    }

    mem_list = mem_pool;

    int free_slot_pos = bitmap_find_first_bit(mem_list->free_slots_bitmap, mem_list->bitmap_size, 1);
    
    if(free_slot_pos == BITMAP_OP_NOT_FOUND)
    {
        STRU_MEM_BATCH * new_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
        if(NULL == new_batch)
        {
            printf("ERROR: Failed to allocate memory for new batch\n");
            return NULL;
        }

        new_batch->batch_mem = malloc(mem_list->slot_size * MEM_BATCH_SLOT_COUNT);
        if(NULL == new_batch->batch_mem)
        {
            printf("ERROR: Failed to allocate memory for new batch memory\n");
            free(new_batch);
            return NULL;
        }

        new_batch->next_batch = NULL;
        STRU_MEM_BATCH * last_batch = mem_list->first_batch;
        while(NULL != last_batch->next_batch)
        {
            last_batch = last_batch->next_batch;
        }
        last_batch->next_batch = new_batch;

        int old_bitmap_size = mem_list->bitmap_size;
        mem_list->batch_count++;
        int new_bitmap_size = (mem_list->batch_count * MEM_BATCH_SLOT_COUNT + BIT_PER_BYTE - 1) / BIT_PER_BYTE;

        unsigned char * new_bitmap = (unsigned char *)malloc(new_bitmap_size);
        if(NULL == new_bitmap)
        {
            printf("ERROR: Failed to allocate memory for new bitmap\n");
            return NULL;
        }

        memcpy(new_bitmap, mem_list->free_slots_bitmap, old_bitmap_size);

        memset(new_bitmap + old_bitmap_size, 0xFF, new_bitmap_size - old_bitmap_size);

        free(mem_list->free_slots_bitmap);
        mem_list->free_slots_bitmap = new_bitmap;
        mem_list->bitmap_size = new_bitmap_size;

        free_slot_pos = bitmap_find_first_bit(mem_list->free_slots_bitmap, mem_list->bitmap_size, 1);
    }

    if(free_slot_pos < 0)
    {
        printf("ERROR: Could not find free slot\n");
        return NULL;
    }

    bitmap_clear_bit(mem_list->free_slots_bitmap, mem_list->bitmap_size, free_slot_pos);

    int batch_index = free_slot_pos / MEM_BATCH_SLOT_COUNT;
    int slot_index = free_slot_pos % MEM_BATCH_SLOT_COUNT;

    STRU_MEM_BATCH * target_batch = mem_list->first_batch;
    for(int i = 0; i < batch_index; i++)
    {
        target_batch = target_batch->next_batch;
    }

    void * slot_addr = (char *)target_batch->batch_mem + (slot_index * mem_list->slot_size);

    return slot_addr;
}

/*
 * Free a chunk of memory pointed by ptr
 * Print out any error messages
 * @param: the pointer to the allocated memory slot
 */
void mem_mngr_free(void * ptr)
{
    if(NULL == ptr)
    {
        return;
    }
    
    if(NULL == mem_pool)
    {
        printf("ERROR: Memory manager not initialized\n");
        return;
    }
    
    STRU_MEM_LIST * mem_list = mem_pool;
    int found = 0;

    while(NULL != mem_list && !found)
    {
        STRU_MEM_BATCH * current_batch = mem_list->first_batch;
        int batch_index = 0;

        while(NULL != current_batch && !found)
        {
            char * batch_start = (char *)current_batch->batch_mem;
            char * batch_end = batch_start + (mem_list->slot_size * MEM_BATCH_SLOT_COUNT);
            
            if (ptr >= (void *)batch_start && ptr < (void *)batch_end)
            {
                ptrdiff_t offset = (char *)ptr - batch_start;
                
                if(offset % mem_list->slot_size != 0)
                {
                    printf("ERROR: ptr is not the starting address of any slot\n");
                    return;
                }

                int slot_index = offset / mem_list->slot_size;
                int bit_pos = batch_index * MEM_BATCH_SLOT_COUNT + slot_index;

                int bit_value = bitmap_bit_is_set(mem_list->free_slots_bitmap, mem_list->bitmap_size, bit_pos);
                if(bit_value == 1)
                {
                    printf("ERROR: ptr is the starting address of an unassigned slot - double freeing\n");
                    return;
                }

                bitmap_set_bit(mem_list->free_slots_bitmap, mem_list->bitmap_size, bit_pos);
                found = 1;
                return;
            }

            current_batch = current_batch->next_batch;
            batch_index++;
        }

        mem_list = mem_list->next_list;
    }
    
    if(!found)
    {
        printf("ERROR: ptr is outside of memory managed by the manager\n");
    }
}