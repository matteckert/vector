#include "vector.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

struct vector {
    size_t elementSize;
    size_t capacity;
    size_t length;
    void *storage;
};

void initialize(Vector *vector, size_t capacity, size_t elementSize);
bool atCapacity(Vector *vector);
void growVector(Vector *vector);
void moveVector(Vector *vector, size_t src, size_t dst);
void *getElement(Vector *vector, size_t index);

Vector *Vector_create(size_t capacity, size_t elementSize) {
    assert(capacity > 0);
    assert(elementSize > 0);
    Vector *vector = malloc(sizeof *vector);
    assert(vector);
    initialize(vector, capacity, elementSize);
    return vector;
}

void initialize(Vector *vector, size_t capacity, size_t elementSize) {
    vector->capacity = capacity;
    vector->elementSize = elementSize;
    vector->storage = malloc(vector->capacity * vector->elementSize);
    assert(vector->storage);
}

void Vector_free(Vector *vector) {
    assert(vector);
    free(vector->storage);
    free(vector);
}

size_t Vector_length(Vector *vector) {
    assert(vector);
    return vector->length;
}

void Vector_push(Vector *vector, void *element) {
    assert(vector);
    assert(element);
    Vector_insert(vector, vector->length, element);
}

void *Vector_peek(Vector *vector) {
    assert(vector);
    return Vector_get(vector, vector->length - 1);
}

void *Vector_pop(Vector *vector) {
    assert(vector);
    void *element = Vector_peek(vector);
    Vector_remove(vector, vector->length - 1);
    return element;
}

void Vector_insert(Vector *vector, size_t index, void *element) {
    assert(vector);
    assert(element);
    assert(index <= vector->length);
    if (atCapacity(vector))
        growVector(vector);
    if (index != vector->length)
        moveVector(vector, index, index + 1);
    vector->length++;
    Vector_replace(vector, index, element);
}

bool atCapacity(Vector *vector) {
    return vector->length == vector->capacity;
}

void growVector(Vector *vector) {
    size_t newCapacity = vector->capacity * 2;
    void *temp = realloc(vector->storage, newCapacity * vector->elementSize);
    assert(temp);
    vector->capacity = newCapacity;
    vector->storage = temp;
}

void moveVector(Vector *vector, size_t src, size_t dst) {
    size_t bytesToCopy = (vector->length - src) * vector->elementSize;
    memmove(getElement(vector, dst), getElement(vector, src), bytesToCopy);
}

void Vector_replace(Vector *vector, size_t index, void *element) {
    assert(vector);
    assert(index < vector->length);
    assert(element);
    memcpy(getElement(vector, index), element, vector->elementSize);
}

void *getElement(Vector *vector, size_t index) {
    return ((char*)vector->storage) + index * vector->elementSize;
}

void Vector_remove(Vector *vector, size_t index) {
    assert(vector);
    assert(index < vector->length);
    if (index != vector->length - 1)
        moveVector(vector, index + 1, index);
    vector->length--;
}

void *Vector_get(Vector *vector, size_t index) {
    assert(vector);
    assert(index < vector->length);
    return getElement(vector, index);
}

void Vector_forEach(Vector *vector, void (*function)(int, void *)) {
    for (size_t i = 0; i < vector->length; i++)
        function(i, getElement(vector, i));
}

void printInt(int i, void *n) {
    printf("%d, ", *(int*)n);
}

void print(Vector *vector) {
    printf("[");
    Vector_forEach(vector, printInt);
    printf("\b\b]\n");
}

int main() {
    Vector *v = Vector_create(5, sizeof(int));

    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; i++) {
        Vector_push(v, a+(i / 10));
    }
    print(v);
    int b = 72;
    Vector_insert(v, Vector_length(v), &(int){44});
    Vector_insert(v, Vector_length(v), &b);
    Vector_insert(v, Vector_length(v), &b);
    Vector_insert(v, Vector_length(v), &b);
    Vector_insert(v, Vector_length(v), &b);
    Vector_insert(v, 2, &b);
    b = 2;
    print(v);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 5);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    Vector_remove(v, 0);
    print(v);
    printf("%d\n", *(int*)Vector_pop(v));
    print(v);
    print(v);
    printf("vector length: %zu\n", v->length);
    Vector_free(v);
}


//bool *Vector_contains(Vector *vector, void *element);

// #define ELEM(i) ((char *) (v->elem) + (i) * (v->size))

// struct vector {
//     size_t plen;
//     size_t llen;
//     size_t size;
//     void (*free)(void *);
//     void *elem;
// };

// static bool brim(vector v);
// static bool grow(vector v);

// vector vector_new(size_t hint, size_t size, void (*freefn)(void *)) {
//     vector v = malloc(sizeof *v);
//     if (!v) return NULL;
//     v->plen = hint ? hint : 16;
//     v->llen = 0;
//     v->size = size ? size : 1;
//     v->free = freefn;
//     v->elem = malloc(v->plen * v->size);
//     if (!v->elem) { free(v); return NULL; }
//     return v;
// }

// void vector_free(vector v) {
//     if (v->free)
//         for (size_t i = 0; i < v->llen; i++)
//             v->free(*(void **)(ELEM(i)));
//     free(v->elem);
//     free(v);
// }

// size_t vector_size(vector v) {
//     if (!v) return 0;
//     return v->llen;
// }

// bool vector_empty(vector v) {
//     if (!v) return true;
//     return v->llen ? false : true;
// }

// bool vector_clear(vector v) {
//     if (!v) return false;
//     if (v->free)
//         for (size_t i = 0; i < v->llen; v++)
//             v->free(*(void **)(ELEM(i)));
//     v->llen = 0;
//     return true;
// }

// bool vector_get(vector v, size_t i, void *elem) {
//     if (!v || i >= v->llen || !elem) return false;
//     memcpy(elem, ELEM(i), v->size);
//     return true;
// }

// bool vector_remove(vector v, size_t i, void *elem) {
//     if (!v || i >= v->llen) return false;
//     if (elem) memcpy(elem, ELEM(i), v->size);
//     else if (v->free) v->free(*(void **)(ELEM(i)));
//     memmove(ELEM(i), ELEM(i+1), (v->llen-- - i + v->size));
//     return true;
// }

// bool vector_add(vector v, void *elem) {
//     if (!v) return false;
//     if (brim(v) && !grow(v)) return false;
//     memcpy(ELEM(v->llen++), elem, v->size);
//     return true;
// }

// bool vector_insert(vector v, size_t i, void *elem) {
//     if (!v || i >= v->llen) return false;
//     if (brim(v) && !grow(v)) return false;
//     memmove(ELEM(i+1), ELEM(i), (v->llen++ - i + v->size));
//     memcpy(ELEM(i), elem, v->size);
//     return true;
// }

// static bool brim(vector v) { return v->llen == v->plen ? true : false; }

// static bool grow(vector v) {
//     size_t more = v->plen * 2;
//     void *temp = realloc(v->elem, more * v->size);
//     if (!temp) return false;
//     v->plen = more;
//     v->elem = temp;
//     return true;
// }
