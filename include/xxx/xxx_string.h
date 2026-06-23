#ifndef XXX_STRING_H
#define XXX_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef XXX_STRING_DEBUG
#  ifdef DEBUG
#    define XXX_STRING_DEBUG 1
#  else
#    define XXX_STRING_DEBUG 0
#  endif
#endif

#include "xxx_assert.h"

#ifndef XXX_STRING_ASSERT
#  define XXX_STRING_ASSERT XXX_ASSERT
#endif

#include "xxx_allocator.h"

#ifndef XXX_STRING_ALLOCATOR
#  define XXX_STRING_FREE    XXX_FREE
#  define XXX_STRING_MALLOC  XXX_MALLOC
#  define XXX_STRING_REALLOC XXX_REALLOC
#endif

#define XXX_STRING_CAPACITY_MAX ((size_t)0x7ffffffe)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_string xxx_string_t;

static inline int xxx_string_init(xxx_string_t *self);
static inline void xxx_string_deinit(xxx_string_t *self);
static inline int xxx_string_copy(xxx_string_t *dst, const xxx_string_t *src);
static inline void xxx_string_move(xxx_string_t *dst, xxx_string_t *src);
static inline size_t xxx_string_length(const xxx_string_t *self);
static inline size_t xxx_string_capacity(const xxx_string_t *self);
static inline bool xxx_string_empty(const xxx_string_t *self);
static inline char *xxx_string_cstr(xxx_string_t *self);
static inline char *xxx_string_at(xxx_string_t *self, size_t i);
static inline char *xxx_string_front(xxx_string_t *self);
static inline char *xxx_string_back(xxx_string_t *self);
static inline int xxx_string_reserve(xxx_string_t *self, size_t n);
static inline int xxx_string_assign(xxx_string_t *self, const char *s, size_t n);
static inline int xxx_string_append(xxx_string_t *self, const char *s, size_t n);
static inline int xxx_string_pushback(xxx_string_t *self, char c);
static inline void xxx_string_popback(xxx_string_t *self);
static inline void xxx_string_clear(xxx_string_t *self);

#ifdef __cplusplus
}
#endif

typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} xxx_long_string_t;

typedef struct {
    char buf[sizeof(xxx_long_string_t) - 1];
    uint8_t len;
} xxx_short_string_t;

struct xxx_string {
    union {
        xxx_short_string_t sstr;
        xxx_long_string_t lstr;
    };
};

static inline
int xxx_long_string_grow(xxx_long_string_t *self, size_t new_cap) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(
        new_cap > self->cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->cap);
    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_STRING_CAPACITY_MAX);
#endif
    char *new_buf = (char *)XXX_STRING_REALLOC(self->buf, new_cap + 1);
    if (new_buf == NULL)
        return -1;
    self->buf = new_buf;
    self->cap = new_cap;
    return 0;
}

static inline
int xxx_short_string_grow(xxx_short_string_t *self, size_t new_cap) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(
        new_cap > 22,
        "new capacity %zu must be greater than current capacity 22", new_cap);
    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_STRING_CAPACITY_MAX);
#endif
    char *new_buf = (char *)XXX_STRING_MALLOC(new_cap + 1);
    if (new_buf == NULL)
        return -1;
    size_t len = self->len & 0x7f;
    memcpy(new_buf, self->buf, len);
    xxx_long_string_t *str = (xxx_long_string_t *)self;
    str->buf = new_buf;
    str->len = len;
    str->cap = new_cap;
    return 0;
}

static inline
int xxx_string_isshort(const xxx_string_t *self) {
    return (self->sstr.len & 0x80) != 0;
}

static inline
int xxx_string_init(xxx_string_t *self) {
    self->sstr.len = 0x80;
    return 0;
}

static inline
void xxx_string_deinit(xxx_string_t *self) {
    if (xxx_string_isshort(self))
        return;
    XXX_STRING_FREE(self->lstr.buf);
    self->lstr.buf = NULL;
}

static inline
int xxx_string_copy(xxx_string_t *dst, const xxx_string_t *src) {
    if (dst == src)
        return 0;
    if (xxx_string_isshort(src)) {
        if (!xxx_string_isshort(dst)) {
            XXX_STRING_FREE(dst->lstr.buf);
        }
        *dst = *src;
        return 0;
    }
    size_t len = src->lstr.len;
    if (xxx_string_isshort(dst)) {
        char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
        if (new_buf == NULL)
            return -1;
        dst->lstr.buf = new_buf;
        dst->lstr.cap = len;
    } else if (dst->lstr.cap < len) {
        char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
        if (new_buf == NULL)
            return -1;
        XXX_STRING_FREE(dst->lstr.buf);
        dst->lstr.buf = new_buf;
        dst->lstr.cap = len;
    }
    memcpy(dst->lstr.buf, src->lstr.buf, len);
    dst->lstr.len = len;
    return 0;
}

static inline
void xxx_string_move(xxx_string_t *dst, xxx_string_t *src) {
    if (!xxx_string_isshort(dst)) {
        XXX_STRING_FREE(dst->lstr.buf);
    }
    *dst = *src;
    src->lstr.buf = NULL;
}

static inline
size_t xxx_string_length(const xxx_string_t *self) {
    if (xxx_string_isshort(self))
        return self->sstr.len & 0x7f;
    return self->lstr.len;
}

static inline
size_t xxx_string_capacity(const xxx_string_t *self) {
    if (xxx_string_isshort(self))
        return 22;
    return self->lstr.cap;
}

static inline
bool xxx_string_empty(const xxx_string_t *self) {
    return xxx_string_length(self) == 0;
}

static inline
char *xxx_string_cstr(xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        xxx_short_string_t *str = &self->sstr;
        str->buf[str->len & 0x7f] = '\0';
        return str->buf;
    }
    xxx_long_string_t *str = &self->lstr;
    str->buf[str->len] = '\0';
    return str->buf;
}

static inline
char *xxx_string_at(xxx_string_t *self, size_t i) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(
        i < xxx_string_length(self),
        "index %zu out of range [0, %zu)", i, xxx_string_length(self));
#endif
    if (xxx_string_isshort(self))
        return &self->sstr.buf[i];
    return &self->lstr.buf[i];
}

static inline
char *xxx_string_front(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif
    if (xxx_string_isshort(self))
        return &self->sstr.buf[0];
    return &self->lstr.buf[0];
}

static inline
char *xxx_string_back(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif
    if (xxx_string_isshort(self)) {
        xxx_short_string_t *str = &self->sstr;
        return &str->buf[(str->len & 0x7f) - 1];
    }
    xxx_long_string_t *str = &self->lstr;
    return &str->buf[str->len - 1];
}

static inline
int xxx_string_reserve(xxx_string_t *self, size_t n) {
    if (n > XXX_STRING_CAPACITY_MAX)
        return -1;
    if (xxx_string_isshort(self)) {
        xxx_short_string_t *str = &self->sstr;
        if (n <= 22)
            return 0;
        return xxx_short_string_grow(str, n);
    }
    xxx_long_string_t *str = &self->lstr;
    if (n <= str->cap)
        return 0;
    return xxx_long_string_grow(str, n);
}

static inline
int xxx_string_assign(xxx_string_t *self, const char *s, size_t n) {
    bool is_short = xxx_string_isshort(self);
    if (is_short) {
        xxx_short_string_t *str = &self->sstr;
        if (n <= 22) {
            memcpy(str->buf, s, n);
            str->len = n | 0x80;
            return 0;
        }
    } else {
        xxx_long_string_t *str = &self->lstr;
        if (n <= str->cap) {
            memcpy(str->buf, s, n);
            str->len = n;
            return 0;
        }
    }
    if (n > XXX_STRING_CAPACITY_MAX)
        return -1;
    char *new_buf = (char *)XXX_STRING_MALLOC(n + 1);
    if (new_buf == NULL)
        return -1;
    memcpy(new_buf, s, n);
    xxx_long_string_t *str = &self->lstr;
    if (!is_short) {
        XXX_STRING_FREE(str->buf);
    }
    str->buf = new_buf;
    str->len = n;
    str->cap = n;
    return 0;
}

static inline
int xxx_string_append(xxx_string_t *self, const char *s, size_t n) {
    char *old_buf;
    if (xxx_string_isshort(self)) {
        xxx_short_string_t *str = &self->sstr;
        size_t len = str->len & 0x7f;
        if (n > XXX_STRING_CAPACITY_MAX - len)
            return -1;
        old_buf = str->buf;
        size_t new_len = len + n;
        if (new_len <= 22) {
            memcpy(str->buf + len, s, n);
            str->len += n;
            return 0;
        }
        size_t new_cap = 44;
        if (new_cap < new_len) {
            new_cap = new_len;
        }
        if (xxx_short_string_grow(str, new_cap) != 0)
            return -1;
    } else {
        xxx_long_string_t *str = &self->lstr;
        if (n > XXX_STRING_CAPACITY_MAX - str->len)
            return -1;
        old_buf = str->buf;
        size_t new_len = str->len + n;
        if (new_len > str->cap) {
            size_t new_cap = str->cap << 1;
            if (new_cap < new_len || new_cap > XXX_STRING_CAPACITY_MAX) {
                new_cap = new_len;
            }
            if (xxx_long_string_grow(str, new_cap) != 0)
                return -1;
        }
    }
    xxx_long_string_t *str = &self->lstr;
    if (s >= old_buf && s < old_buf + str->len) {
        s = str->buf + (s - old_buf);
    }
    memcpy(str->buf + str->len, s, n);
    str->len += n;
    return 0;
}

static inline
int xxx_string_pushback(xxx_string_t *self, char c) {
    if (xxx_string_isshort(self)) {
        xxx_short_string_t *str = &self->sstr;
        size_t len = str->len & 0x7f;
        if (len < 22) {
            str->buf[len] = c;
            ++str->len;
            return 0;
        }
        if (xxx_short_string_grow(str, 44) != 0)
            return -1;
    } else {
        xxx_long_string_t *str = &self->lstr;
        if (str->len == str->cap) {
            if (str->cap == XXX_STRING_CAPACITY_MAX)
                return -1;
            size_t new_cap = str->cap << 1;
            if (new_cap > XXX_STRING_CAPACITY_MAX)
                new_cap = XXX_STRING_CAPACITY_MAX;
            if (xxx_long_string_grow(str, new_cap) != 0)
                return -1;
        }
    }
    xxx_long_string_t *str = &self->lstr;
    str->buf[str->len++] = c;
    return 0;
}

static inline
void xxx_string_popback(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif
    if (xxx_string_isshort(self)) {
        --self->sstr.len;
        return;
    }
    --self->lstr.len;
}

static inline
void xxx_string_clear(xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        self->sstr.len = 0x80;
        return;
    }
    self->lstr.len = 0;
}

#endif
