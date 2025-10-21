#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#include <inttypes.h>
#include <stdbool.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
// ...

typedef float f32;
typedef double f64;

typedef enum {
	TYPE_I32,
	TYPE_U32,
	TYPE_I64,
	TYPE_U64,
	TYPE_F32,
	TYPE_F64,
	TYPE_CHAR,
	TYPE_PTR
} DataType;

typedef union {
	i32 i32Value;
	u32 u32Value;
	i64 i64Value;
	u64 u64Value;
	f32 f32Value;
	f64 f64Value;
	char charValue;
	void *ptrValue;
} GenericValue;

#endif //__DATATYPE_H__ 
