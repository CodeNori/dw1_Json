#pragma once
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <deque>
#endif

typedef struct Value_t
{
    int type;
    struct Value_t* next;

    union {
        const char* sval;
        double fval;
        struct Object_t* obj;
        struct Array_t* arr;
    };

} Value_t;


typedef struct Pair_t
{
    const char* key;
    Value_t* value;
    struct Pair_t* next;

#ifdef __cplusplus
    operator bool() const noexcept(false);
    operator int() const noexcept(false) { return (int)value->fval; }
    operator float() const noexcept(false) { return value->fval; }
    operator double() const noexcept(false) { return value->fval; }
    operator const char* () const noexcept(false) { return value->sval; }
    operator struct Array_t* () const noexcept(false) { return value->arr; }
    operator struct Object_t*() const noexcept(false) { return value->obj; }

    Pair_t& operator[](const char* name);

#endif

} Pair_t;

typedef struct Array_t
{
    Value_t* start;

#ifdef __cplusplus
    int getX(float* name);
    Array_t* getY(int y);
#endif

} Array_t;

typedef struct Object_t
{
    Pair_t* start;

#ifdef __cplusplus
    Pair_t* Find(const char* name);
    Pair_t& operator[](const char* name);

#endif

} Object_t;



typedef struct MemoryPosition1
{
    char* start;
    char* end;
    char* curr;
} MemoryPosition1;


typedef struct JsonFile
{
    Object_t* mRoot;
    MemoryPosition1 strBuf;

#ifdef __cplusplus

    std::deque< Object_t> objList;
    std::deque< Array_t> arrList;
    std::deque< Pair_t> pairList;
    std::deque< Value_t> valueList;


    ~JsonFile();
    Pair_t* Find(const char* name);
    Pair_t& operator[](const char* name)
    {
        return (*mRoot)[name];
    }
    char* new_string(char* s, int len);

    void Print(FILE* forg);
#endif

} JsonFile;



int CreateJSON(JsonFile* json, const char* file_name);



#ifdef __cplusplus
    extern "C" {
#endif

    void Print_Val(Value_t* a, int level);
    void Print_Pair(Pair_t* a, int level);
    void Print_Arr(Array_t* a, int level);
    void Print_Obj(Object_t* a, int level);

    int Load_File(MemoryPosition1* membuf, const char* file_name);
        
    char* new_strcopy(char* s, int len);
    Value_t* new_Value_t(int t);
    Pair_t* new_Pair_t(const char* k, Value_t* v);
    Array_t* new_Array_t();
    Object_t* new_Object_t();

#ifdef __cplusplus
    }
#endif



    /*
    yylex_destroy(); 

    */