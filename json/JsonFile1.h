#pragma once
#include <vector>
#include <string>
#include <deque>
#include "JsonFileBase.h"

namespace dw1
{

	class JsonFile1 : public JsonFileBase
	{
		bool process_Brace0();
		bool process_Bracket0();
		bool process_KeyValue();
		bool process_Blank_Comma();
		bool process_Brace0_KeyValue_Comma();
		bool process_Bracket0_Value_Comma();

		bool process_Blank_Bracket1();
		bool process_Blank_Brace1();
		bool process_Brace0_KeyValue_Brace1();
		bool process_Bracket0_Value_Bracket1();

		bool Bracket0_Value_T0();
		bool Brace0_KeyValue_T0();

	public:

		void Parse(const char* file_name);
	};





}

