雖然現在記憶體價錢已經很低，但在一些 IOT 或是傳感器上記憶體的量還是很少，所以除了算法要去優化外，記憶體也不可能一直申請在釋放，通常一次要就會要一大塊，然後在整個一起還回去來降低開銷，也可以避免記憶體碎片化。當然我們也可以直接在 STACK 上直接開一塊很大的記憶體，但通常 STACK 預設很小，所以一般會在 HEAP 上去申請。而實際情況還需要考慮多線程的情況，這邊就先從單線程的例子開始，在漸漸地往多線程。

## 一、單線程記憶體池
雖然我們是一次要一大塊 Memory，但這一大塊中還會再切分成很多小塊 block_count，其中每一小塊大小為 block_size，最後並將頭尾連接，這樣就可以比較方便去控管每一小塊的使用情況。如果新進來的資料還可以放進可用塊中，那就先使用可用塊，否則就往下一小塊中去尋找。
#### 1. 資料結構
```C
typedef struct MemoryBlock {
    struct MemoryBlock* next;
} MemoryBlock;

typedef struct {
    MemoryBlock* free_list;  // 自由列表（可用塊）
    char* pool_start;        // 內存池起始指針
    size_t block_size;       // 內存塊大小（可由使用者決定）
    size_t block_count;      // 內存池內存塊數量
    size_t offset;           // 當前分配位置（用於線性分配）
} MemoryPool;
```
所以我們申請的總大小為 block_size*block_count，offset 則是看目前申請的記憶體位置是否還足夠用，如果不夠就需要再去申請另外一塊記憶體，並且將申請的這兩塊用 linked list 的方式連接起來
#### 2. 初始化
```C
MemoryPool* init_pool(size_t block_size, size_t block_count) {
    MemoryPool* mp = (MemoryPool*)malloc(sizeof(MemoryPool));
    if (!mp) return NULL;

    mp->pool_start = (char*)malloc(block_size * block_count);
    if (!mp->pool_start) {
        free(mp);
        return NULL;
    }

    mp->block_size = block_size;
    mp->block_count = block_count;
    mp->free_list = NULL;
    mp->offset = 0;
    return mp;
}
```
其中 ```mp->free_list = NULL;``` 是因為還沒使用，若是有申請另一塊就需要去跟其他塊連接。
#### 3. 將資料放入 POOL
```C
void* pool_alloc(MemoryPool* mp) {
    if (mp->free_list) {
        // 使用自由列表中的可用塊
        MemoryBlock* allocated = mp->free_list;
        mp->free_list = allocated->next;
        return allocated;
    } else if (mp->offset + mp->block_size <= mp->block_size * mp->block_count) {
        // 線性分配新的塊
        void* ptr = mp->pool_start + mp->offset;
        mp->offset += mp->block_size;
        return ptr;
    }
    return NULL; // 內存池已滿
}
```
如果得到 NULL，那就必須去在要一塊，否則就可以一直將資料放入。
#### 4. 釋放某一塊記憶體
```C
void pool_free(MemoryPool* mp, void* block) {
    if (!block) return;
    MemoryBlock* freed = (MemoryBlock*)block;
    freed->next = mp->free_list;
    mp->free_list = freed; // 把釋放的塊放回自由列表
}
```
當某一塊記憶體已用完時我們換需要將其釋放，所以必須把該塊記憶體的位置傳入，清空之後就可以重新使用該塊記憶體。
#### 5. 重置與銷毀
```C
void pool_reset(MemoryPool* mp) {
    mp->free_list = NULL; // 清空自由列表
    mp->offset = 0;       // 重置線性分配
}

void destroy_pool(MemoryPool* mp) {
    free(mp->pool_start);
    free(mp);
}
```
最後的使用就是
```C
int main() {
    size_t user_defined_block_size = 8; // 由使用者決定塊大小
    size_t user_defined_block_count = 32; // 由使用者決定塊數量
    MemoryPool* mp = init_pool(user_defined_block_size, user_defined_block_count);

    if (!mp) {
        printf("內存池初始化失敗\n");
        return 1;
    }

    int *a = (int *)pool_alloc(mp);
    if (a) {
        *a = 42; // 賦值
        printf("Allocated value: %d, %p\n", *a, a);
    }
    int *b = (int *)pool_alloc(mp);
    if (b) {
        *b = 42; // 賦值
        printf("Allocated value: %d, %p\n", *b, b);
    }
    int *c = (int *)pool_alloc(mp);
    if (c) {
        *c = 42; // 賦值
        printf("Allocated value: %d, %p\n", *c, c);
    }
    pool_free(mp, b); // **釋放 a**
    void* d = pool_alloc(mp); // **重新使用 a 的位置**
    printf("重新分配的內存: %p\n", d);
    
    pool_reset(mp);
    destroy_pool(mp);

    return 0;
}
```

