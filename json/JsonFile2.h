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

		bool Parse(const char* file_name);

		jsonKeyValue& operator[](const char* name)
		{
			return mRoot->at(name);
		}

	};


}

