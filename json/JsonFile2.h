#pragma once
#include <vector>
#include <string>
#include <deque>
#include "JsonFileBase.h"


namespace dw1
{


	class JsonFile2 : public JsonFileBase
	{
		bool stmt_Object();
		bool stmt_Value();
		bool stmt_Array();

		void make_KeyValue();

	public:
		JsonFile2() {}
		JsonFile2(const char* file_name) { Parse(file_name); }
		bool Parse(const char* file_name);

		jsonKeyValue& operator[](const char* name)
		{
			return mRoot->at(name);
		}

	};


}

