#include <stdio.h>
#include "lua_fast_impl.h"

#define IS_INT_TYPE_EQUIVALENT(x, T) (!isinf(x) && (T)(x) == (x))

#define IS_INT64_EQUIVALENT(x) IS_INT_TYPE_EQUIVALENT(x, int64_t)
#define IS_INT_EQUIVALENT(x) IS_INT_TYPE_EQUIVALENT(x, int)

int fmp_traverse(const Table *t, foldfunc fold, void *cargo) {
  int goon = fmp_hash_part(t, fold, (void *)cargo);
  if (!goon) return goon;
  goon = fmp_array_part(t, fold, cargo);
  return goon;
}

#if LUAJIT_VERSION_NUM == 20100

struct array_check_data {
  uint32_t max;
  uint32_t len;
};

static int fmp_check_array_fold(uint32_t ik, TValue *k, TValue *o,
                                void *cargo) {
  struct array_check_data *d = cargo;
  if (k != NULL) {
    if (XLUA_IS_NUMBER(k)) {
      lua_Number n = XLUA_NUMBER(k);
      if (n >= 1 && IS_INT_EQUIVALENT(n)) {
        ik = (uint32_t)n;
        goto ret;
      }
    }
  } else {
    goto ret;
  }
  return 0;
ret:
  d->len++;
  d->max = d->max > ik ? d->max : ik;
  return 1;
}

int fmp_len_if_array(const Table *t) {
  struct array_check_data data;
  data.max = 0;
  data.len = 0;
  if (!fmp_traverse(t, fmp_check_array_fold, &data)) {
    return 0;
  } else if (data.max == data.len) {
    return data.max;
  } else {
    return 0;
  }
}

static int fmp_array_fill_fold(uint32_t ik, TValue *k, TValue *o, void *cargo) {
  TValue **out = cargo;
  if (k != NULL) {
    ik = (uint32_t)XLUA_NUMBER(k);
  }
  out[ik - 1] = o;
  return 1;
}

int fmp_fill_array(const Table *t, TValue **out) {
  fmp_traverse(t, fmp_array_fill_fold, out);
  return 1;
}

static int fmp_map_len_fold(uint32_t ik, TValue *k, TValue *o, void *cargo) {
  int *len = cargo;
  (*len)++;
  return 1;
}

int fmp_map_len(const Table *t) {
  int len = 0;
  fmp_traverse(t, fmp_map_len_fold, &len);
  return len;
}

int fmp_array_part(const Table *t, foldfunc fold, void *cargo) {
  uint32_t i, b;
  if (t->asize == 0) return 1;  // =: continue
  for (i = b = 0; b < LJ_MAX_ABITS; b++) {
    uint32_t n, top = 2u << b;
    TValue *array;
    if (top >= t->asize) {
      top = t->asize - 1;
      if (i > top) break;
    }
    array = tvref(t->array);
    for (n = 0; i <= top; i++) {
      TValue *v = &array[i]; /* i, not i-1, as in the Lua 5.1 part  */
      if (!tvisnil(v)) {
        if (!(*fold)(i, NULL, v, cargo)) return 0;  // =: break
      }
    }
  }
  return 1;  // =: continue
}

int fmp_hash_part(const Table *t, foldfunc fold, void *cargo) {
  uint32_t i, hmask = t->hmask;
  Node *node = noderef(t->node);
  for (i = 0; i <= hmask; i++) {
    Node *n = &node[i];
    if (!tvisnil(&n->val) && !tvisnil(&n->key)) {
      if (!(*fold)(0, (&n->key), (&n->val), cargo)) return 0;  // =: break
    }
  }
  return 1;  // =: continue
}

TValue *mp_index2addr(lua_State *L, int idx) {
  if (idx > 0) {
    TValue *o = L->base + (idx - 1);
    return o < L->top ? o : niltv(L);
  } else {
    api_check(L, idx != 0 && -idx <= L->top - L->base);
    return L->top + idx;
  }
}

#endif
