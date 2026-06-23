#ifndef XXX_STACK_H
#define XXX_STACK_H

#include "xxx_array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_stack xxx_stack_t;

static inline int xxx_stack_init(xxx_stack_t *self);
static inline void xxx_stack_deinit(xxx_stack_t *self);
static inline int xxx_stack_copy(xxx_stack_t *dst, const xxx_stack_t *src);
static inline void xxx_stack_move(xxx_stack_t *dst, xxx_stack_t *src);
static inline size_t xxx_stack_size(const xxx_stack_t *self);
static inline bool xxx_stack_empty(const xxx_stack_t *self);
static inline void **xxx_stack_peek(xxx_stack_t *self);
static inline int xxx_stack_push(xxx_stack_t *self, void *x);
static inline void xxx_stack_pop(xxx_stack_t *self);
static inline void xxx_stack_clear(xxx_stack_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_stack {
    xxx_array_t arr;
};

static inline
int xxx_stack_init(xxx_stack_t *self) {
    return xxx_array_init(&self->arr);
}

static inline
void xxx_stack_deinit(xxx_stack_t *self) {
    xxx_array_deinit(&self->arr);
}

static inline
int xxx_stack_copy(xxx_stack_t *dst, const xxx_stack_t *src) {
    return xxx_array_copy(&dst->arr, &src->arr);
}

static inline
void xxx_stack_move(xxx_stack_t *dst, xxx_stack_t *src) {
    xxx_array_move(&dst->arr, &src->arr);
}

static inline
size_t xxx_stack_size(const xxx_stack_t *self) {
    return xxx_array_length(&self->arr);
}

static inline
bool xxx_stack_empty(const xxx_stack_t *self) {
    return xxx_array_empty(&self->arr);
}

static inline
void **xxx_stack_peek(xxx_stack_t *self) {
    return xxx_array_back(&self->arr);
}

static inline
int xxx_stack_push(xxx_stack_t *self, void *x) {
    return xxx_array_pushback(&self->arr, x);
}

static inline
void xxx_stack_pop(xxx_stack_t *self) {
    xxx_array_popback(&self->arr);
}

static inline
void xxx_stack_clear(xxx_stack_t *self) {
    xxx_array_clear(&self->arr);
}

#endif
