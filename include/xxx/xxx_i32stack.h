#ifndef XXX_i32stack_H
#define XXX_i32stack_H

#include "xxx_i32array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32stack xxx_i32stack_t;

static inline int xxx_i32stack_init(xxx_i32stack_t *self);
static inline void xxx_i32stack_deinit(xxx_i32stack_t *self);
static inline int xxx_i32stack_copy(xxx_i32stack_t *dst, const xxx_i32stack_t *src);
static inline void xxx_i32stack_move(xxx_i32stack_t *dst, xxx_i32stack_t *src);
static inline size_t xxx_i32stack_size(const xxx_i32stack_t *self);
static inline bool xxx_i32stack_empty(const xxx_i32stack_t *self);
static inline int *xxx_i32stack_peek(xxx_i32stack_t *self);
static inline int xxx_i32stack_push(xxx_i32stack_t *self, int x);
static inline void xxx_i32stack_pop(xxx_i32stack_t *self);
static inline void xxx_i32stack_clear(xxx_i32stack_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32stack {
    xxx_i32array_t arr;
};

static inline
int xxx_i32stack_init(xxx_i32stack_t *self) {
    return xxx_i32array_init(&self->arr);
}

static inline
void xxx_i32stack_deinit(xxx_i32stack_t *self) {
    xxx_i32array_deinit(&self->arr);
}

static inline
int xxx_i32stack_copy(xxx_i32stack_t *dst, const xxx_i32stack_t *src) {
    return xxx_i32array_copy(&dst->arr, &src->arr);
}

static inline
void xxx_i32stack_move(xxx_i32stack_t *dst, xxx_i32stack_t *src) {
    xxx_i32array_move(&dst->arr, &src->arr);
}

static inline
size_t xxx_i32stack_size(const xxx_i32stack_t *self) {
    return xxx_i32array_length(&self->arr);
}

static inline
bool xxx_i32stack_empty(const xxx_i32stack_t *self) {
    return xxx_i32array_empty(&self->arr);
}

static inline
int *xxx_i32stack_peek(xxx_i32stack_t *self) {
    return xxx_i32array_back(&self->arr);
}

static inline
int xxx_i32stack_push(xxx_i32stack_t *self, int x) {
    return xxx_i32array_pushback(&self->arr, x);
}

static inline
void xxx_i32stack_pop(xxx_i32stack_t *self) {
    xxx_i32array_popback(&self->arr);
}

static inline
void xxx_i32stack_clear(xxx_i32stack_t *self) {
    xxx_i32array_clear(&self->arr);
}

#endif
