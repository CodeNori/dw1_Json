#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "json_type.h"


extern "C" {

    extern MemoryPosition1 src_buf;
    extern Object_t* gJson;
    void yyparse();

}

JsonFile* current_jsonfile;

static const char* null_string = "<null>";
Value_t null_value = { TOKEN_NULL, 0,  null_string };
Pair_t null_pair = { nullptr, &null_value, nullptr };
FILE* fp_print;


int Array_t::getX(float* arr)
{
    if (!start) return 0;

    int i = 0;
    Value_t* p = start;
    while (p) {
        arr[i] = p->fval;
        p = p->next;
        ++i;
    }
    return i;
}

Array_t* Array_t::getY(int y)
{
    if (!start) return 0;

    int i = 0;
    Value_t* p = start;
    while (p) {
        if (i == y) return p->arr;
        p = p->next;
        ++i;
    }
    return nullptr;
}


Pair_t* Object_t::Find(const char* name)
{
    Pair_t* p = start;

    while (p) {
        if (strcmp(p->key, name) == 0) return p;
        if (p->value->type == TOKEN_OBJECT) {
            Pair_t* n = p->value->obj->Find(name);
            if (n) return n;
        }
        p = p->next;
    }

    return nullptr;
}

Pair_t& Object_t::operator[](const char* name)
{
    Pair_t* p = Find(name);
    if (p) return *p;

    return null_pair;
}

Pair_t& Pair_t::operator[](const char* name)
{
    if (value->type == TOKEN_OBJECT) {
        Pair_t* n = value->obj->Find(name);
        if (n) return *n;
    }

    return null_pair;
}

Pair_t::operator bool() const
{
    if (value->type == TOKEN_TRUE)
        return true;
    else return false;
}


JsonFile::~JsonFile()
{
    if (strBuf.start) {
        ::free(strBuf.start);

        strBuf.start = 0;
        strBuf.end = 0;
        strBuf.curr = 0;
    }
}

Pair_t* JsonFile::Find(const char* name)
{
    return mRoot->Find(name);
}

char* JsonFile::new_string(char* s, int size)
{
    char* str = strBuf.curr;
    int len = size - 2;
    strncpy_s(str, size, s + 1, len);
    str[len] = 0;
    strBuf.curr += (len + 1);
    return str;
}

void JsonFile::Print(FILE* forg)
{
    fp_print = forg;

    Print_Obj(mRoot, 1);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int CreateJSON(JsonFile* json, const char* file_name)
{
    current_jsonfile = json;
    int size = Load_File(&src_buf, file_name);

    current_jsonfile->strBuf.start = src_buf.start; // (char*)::malloc(size);
    current_jsonfile->strBuf.curr = current_jsonfile->strBuf.start;
    current_jsonfile->strBuf.end = current_jsonfile->strBuf.start + size;

    yyparse();
    current_jsonfile->mRoot = gJson;

    // ::free(src_buf.start);

    return 0;
}


#ifdef __cplusplus
extern "C" {
#endif


    void Print_Val(Value_t* a, int level)
    {
        switch (a->type) {
        case TOKEN_NUMBER: fprintf(fp_print, "%f ", a->fval); break;

        case TOKEN_NULL:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
        case TOKEN_STRING: fprintf(fp_print, "\"%s\" ", a->sval); break;

        case TOKEN_OBJECT: Print_Obj(a->obj, level + 1); break;
        case TOKEN_ARRAY: Print_Arr(a->arr, level + 1); break;

        }
    }


    void Print_Pair(Pair_t* a, int level)
    {
        fprintf(fp_print, "\"%s\" : ", a->key);
        Print_Val(a->value, level + 1);
    }


    void Print_Obj(Object_t* a, int level)
    {
        if (!a->start) {
            fprintf(fp_print, "{}");
            return;
        }


         fprintf(fp_print, "{\n");

        Pair_t* p = a->start;

        while (p) {
            for (int i = 0; i < level; ++i) fprintf(fp_print, "\t");
            Print_Pair(p, level + 1);
            p = p->next;
            if (p) fprintf(fp_print, ",\n");
        }
        
        fprintf(fp_print, "\n");
        for (int i = 0; i < level-1; ++i) fprintf(fp_print, "\t");
        fprintf(fp_print, "}");

    }


    void Print_Arr(Array_t* a, int level)
    {
        if (!a->start) {
            fprintf(fp_print, "[]");
            return;
        }

        fprintf(fp_print, "[\n");
        for (int i = 0; i < level; ++i) fprintf(fp_print, "\t");

        Value_t* p = a->start;
        while (p) {
            Print_Val(p, level + 1);
            p = p->next;
            if (p) fprintf(fp_print, ", ");
        }

        fprintf(fp_print, "\n");
        for (int i = 0; i < level - 1; ++i) fprintf(fp_print, "\t");
        fprintf(fp_print, "]");

    }



    char* new_strcopy(char* s, int len)
    {
        return current_jsonfile->new_string(s, len);
    }

    int Load_File(MemoryPosition1* membuf, const char* file_name)
    {
        FILE* f = nullptr;
        fopen_s(&f, file_name, "rb");
        if (f == NULL)
            return 0;
        fseek(f, 0, SEEK_END);
        size_t file_size = (size_t)ftell(f);
        if (file_size == -1)
            return 0;
        fseek(f, 0, SEEK_SET);
        char* mFileData = (char*)::malloc(file_size + 2);
        fread(mFileData, 1, file_size, f);
        fclose(f);

        mFileData[file_size] = '\0';
        mFileData[file_size + 1] = '\0';

        membuf->start = mFileData;
        membuf->end = mFileData + file_size;
        membuf->curr = mFileData;

        return (int)file_size;
    }








    Value_t* new_Value_t(int t)
    {
        Value_t v;
        v.type = t;
        v.next = 0;
        v.sval = null_string;

        current_jsonfile->valueList.push_back(v);
        return &current_jsonfile->valueList.back();
    }


    Pair_t* new_Pair_t(const char* k, Value_t* val)
    {
        Pair_t v;
        v.key = k;
        v.value = val;
        v.next = 0;

        current_jsonfile->pairList.push_back(v);
        return &current_jsonfile->pairList.back();
    }


    Array_t* new_Array_t()
    {
        Array_t v;
        v.start = 0;
        current_jsonfile->arrList.push_back(v);
        return &current_jsonfile->arrList.back();
    }


    Object_t* new_Object_t()
    {
        Object_t v;
        v.start = 0;
        current_jsonfile->objList.push_back(v);
        return &current_jsonfile->objList.back();
    }




#ifdef __cplusplus
    }
#endif

