# Custom Memory Allocator

A simple custom memory allocator written in C to understand how dynamic memory allocation works internally.

The project implements `my_malloc()` and `my_free()` using a linked-list based free list and Linux `sbrk()`.

## Project Structure

```text
my_allocator/
├── include/
│   ├── allocator.h
│   ├── my_malloc.h
│   └── my_free.h
├── src/
│   ├── my_malloc.c
│   └── my_free.c
├── tests/
│   └── main.c
├── Makefile
└── README.md
```

## How It Works

Each allocated block contains metadata followed by the memory available to the program:

```text
+-------------------+-------------------+
| Block Metadata    | User Memory       |
+-------------------+-------------------+
```

`my_malloc()` searches the free list for a suitable block. If none is available, it requests more memory using `sbrk()`.

`my_free()` returns the block to the free list. Adjacent free blocks can be merged to reduce fragmentation.

## Build & Run

Compile:

```bash
make
```

Run tests:

```bash
make run
```

Clean build files:

```bash
make clean
```

## Valgrind

Run the allocator under Valgrind:

```bash
make valgrind
```

or:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./allocator_test
```

## Tests

The test program covers:

- Basic allocation
- Multiple allocations
- Freeing and reusing memory
- Block splitting
- Block coalescing
- Zero-size allocation
- `my_free(NULL)`
- Larger allocations
- Valgrind checks

## Results

Final test and Valgrind results will be added after testing the completed implementation.

```text
[ Test output / screenshots ]
```

## Limitations

This is an educational implementation and is not intended to replace a production memory allocator.

- Not thread-safe
- Limited pointer validation
- Uses `sbrk()`
- Not optimized for production performance

## References

- *The C Programming Language* — Kernighan & Ritchie
- Linux `sbrk(2)` manual
- Valgrind Memcheck documentation