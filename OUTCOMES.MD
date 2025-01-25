Assignment is 100% complete.

### Code explanation

The main memory chunks are managed through a linked list implementation. 
Any allocation request is done through passing in a location and size.
The linked list is then checked to make sure that location (and size) are valid.

Each allocation method is responsible for requesting their appropriate locations.
Best and worst both simply calculate each gap size, and find a min/max.

First just grabs the first chunk of valid size.

Any invalid allocations are returned with NULL.

### Summary
#### Test 1
Running a first-fit model in 4096 (0x1000) bytes of memory.
ALLOCATE 80 (+32) bytes (ID 0) - return location 0: ALLOCATE SUCCESS
ALLOCATE 64 (+32) bytes (ID 1) - return location 112: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 2) - return location 208: ALLOCATE SUCCESS
FREE - location 208 (ID 1)
COULD NOT FREE - 7
ALLOCATE 80 (+32) bytes (ID 3) - return location 320: ALLOCATE SUCCESS
ALLOCATE 2048 (+32) bytes (ID 4) - return location 432: ALLOCATE SUCCESS
ALLOCATE 2048 (+32) bytes (ID 5): FAIL
-----------------
Chunk 0 - ID 0: 0-112 ~ 80 bytes
Chunk 1 - ID 2: 208-320 ~ 80 bytes
Chunk 2 - ID 3: 320-432 ~ 80 bytes
Chunk 3 - ID 4: 432-2512 ~ 2048 bytes

This tests both freeing an ID that DNE, allocating more memory that exists, as well as allocating memory with a smaller than requested size chunk available.

#### Test2
Running a first-fit model in 4096 (0x1000) bytes of memory.
ALLOCATE 80 (+32) bytes (ID 0) - return location 0: ALLOCATE SUCCESS
ALLOCATE 64 (+32) bytes (ID 1) - return location 112: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 2) - return location 208: ALLOCATE SUCCESS
FREE - location 208 (ID 1)
ALLOCATE 80 (+32) bytes (ID 3) - return location 320: ALLOCATE SUCCESS
ALLOCATE 32 (+32) bytes (ID 4) - return location 112: ALLOCATE SUCCESS
-----------------
Chunk 0 - ID 0: 0-112 ~ 80 bytes
Chunk 1 - ID 4: 112-176 ~ 32 bytes
Chunk 2 - ID 2: 208-320 ~ 80 bytes
Chunk 3 - ID 3: 320-432 ~ 80 bytes

This tests inserting memory inbetween allocated chunks with appropriate size.

### Test Cases
#### worst
Runs: test3.txt
------------
Running a worst-fit model in 4096 (0x1000) bytes of memory.
ALLOCATE 80 (+32) bytes (ID 0) - return location 0: ALLOCATE SUCCESS
ALLOCATE 128 (+32) bytes (ID 1) - return location 112: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 2) - return location 272: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 3) - return location 384: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 4) - return location 496: ALLOCATE SUCCESS
ALLOCATE 256 (+32) bytes (ID 5) - return location 608: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 6) - return location 896: ALLOCATE SUCCESS
FREE - location 272 (ID 1)
FREE - location 496 (ID 3)
FREE - location 896 (ID 5)
ALLOCATE 32 (+32) bytes (ID 7) - return location 1008: ALLOCATE SUCCESS
-----------------
Chunk 0 - ID 0: 0-112 ~ 80 bytes
Chunk 1 - ID 2: 272-384 ~ 80 bytes
Chunk 2 - ID 4: 496-608 ~ 80 bytes
Chunk 3 - ID 6: 896-1008 ~ 80 bytes
Chunk 4 - ID 7: 1008-1072 ~ 32 bytes

This test case ensures that worst is allocating the chunk that wastes the most size.
In the output, you can see that ID 4 is allocated at the end, where it wastes the most space.

#### Best
Runs: test3.txt
------------
Running a best-fit model in 4096 (0x1000) bytes of memory.
ALLOCATE 80 (+32) bytes (ID 0) - return location 0: ALLOCATE SUCCESS
ALLOCATE 128 (+32) bytes (ID 1) - return location 112: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 2) - return location 272: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 3) - return location 384: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 4) - return location 496: ALLOCATE SUCCESS
ALLOCATE 256 (+32) bytes (ID 5) - return location 608: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 6) - return location 896: ALLOCATE SUCCESS
FREE - location 272 (ID 1)
FREE - location 496 (ID 3)
FREE - location 896 (ID 5)
ALLOCATE 32 (+32) bytes (ID 7) - return location 384: ALLOCATE SUCCESS
-----------------
Chunk 0 - ID 0: 0-112 ~ 80 bytes
Chunk 1 - ID 2: 272-384 ~ 80 bytes
Chunk 2 - ID 7: 384-448 ~ 32 bytes
Chunk 3 - ID 4: 496-608 ~ 80 bytes
Chunk 4 - ID 6: 896-1008 ~ 80 bytes

This test ensures that best model is working correctly.
In the output, you can see that ID 5 is allocated in the least wasteful position.

#### First
Runs: test3.txt
------------
Running a first-fit model in 4096 (0x1000) bytes of memory.
ALLOCATE 80 (+32) bytes (ID 0) - return location 0: ALLOCATE SUCCESS
ALLOCATE 128 (+32) bytes (ID 1) - return location 112: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 2) - return location 272: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 3) - return location 384: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 4) - return location 496: ALLOCATE SUCCESS
ALLOCATE 256 (+32) bytes (ID 5) - return location 608: ALLOCATE SUCCESS
ALLOCATE 80 (+32) bytes (ID 6) - return location 896: ALLOCATE SUCCESS
FREE - location 272 (ID 1)
FREE - location 496 (ID 3)
FREE - location 896 (ID 5)
ALLOCATE 32 (+32) bytes (ID 7) - return location 112: ALLOCATE SUCCESS
-----------------
Chunk 0 - ID 0: 0-112 ~ 80 bytes
Chunk 1 - ID 7: 112-176 ~ 32 bytes
Chunk 2 - ID 2: 272-384 ~ 80 bytes
Chunk 3 - ID 4: 496-608 ~ 80 bytes
Chunk 4 - ID 6: 896-1008 ~ 80 bytes

This test ensures that first model is working correctly.
In the output, you can see that ID 5 is allocated in the first available position.

### Follow Up Questions
which algorithm manages memory in the least overall number of chunks?

Best - in an ideal scenario, there is a free chunk that perfectly fits an allocation request. This reduces
the number of chunks, as it completly fills a free chunk.

which algorithm manages memory with the greatest number of successful allocations?

Best - by wasting the least amount of space, it can handle the most successful allocations.