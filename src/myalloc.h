// Allocates size_t bytes of memory and returns a pointer to the allocated memory.
// Returns NULL on error
void *myalloc(size_t size);

// Frees the given pointer previously allocated by myalloc.
void free(void *ptr);