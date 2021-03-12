#pragma region Ares Copyrights
/*
 *Copyright (c) 2008+, All Ares Contributors
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *3. All advertising materials mentioning features or use of this software
 *   must display the following acknowledgement:
 *   This product includes software developed by the Ares Contributors.
 *4. Neither the name of Ares nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY ITS CONTRIBUTORS ''AS IS'' AND ANY
 *EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *DISCLAIMED. IN NO EVENT SHALL THE ARES CONTRIBUTORS BE LIABLE FOR ANY
 *DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma endregion
#pragma once

#include "../Utilities/Ares.CRT.h"
#include "../Utilities/Debug.h"

#include <algorithm>
#include <memory>
#include <vector>

#include <ArrayClasses.h>
#include <CCINIClass.h>

template <typename T> class Enumerable
{
	typedef std::vector<std::unique_ptr<T>> container_t;
public:
	static container_t Array;

	static int FindIndex(const char *Title)
	{
		auto result = std::find_if(Array.begin(), Array.end(), [Title](std::unique_ptr<T> &Item) {
			return !_strcmpi(Item->Name, Title);
		});
		if(result == Array.end()) {
			return -1;
		}
		return std::distance(Array.begin(), result);
	}

	static T* Find(const char *Title)
	{
		auto result = FindIndex(Title);
		return (result < 0) ? nullptr : Array[static_cast<size_t>(result)].get();
	}

	static T* FindOrAllocate(const char *Title)
	{
		if(T *find = Find(Title)) {
			return find;
		}
		Array.push_back(std::make_unique<T>(Title));
		return Array.back().get();
	}

	static void Clear()
	{
		Array.clear();
	}

	static void LoadFromINIList(CCINIClass *pINI)
	{
		const char *section = GetMainSection();
		int len = pINI->GetKeyCount(section);
		for(int i = 0; i < len; ++i) {
			const char *Key = pINI->GetKeyName(section, i);
			FindOrAllocate(Key);
		}
		for(size_t i = 0; i < Array.size(); ++i) {
			Array[i]->LoadFromINI(pINI);
		}
	}

	static const char * GetMainSection();

	Enumerable(const char* Title) {
		this->Name[0] = 0;

		if(Title) {
			//is this event need stuffs themself ?
			AresCRT::strCopy(this->Name, Title);
			
		}
	}

	virtual ~Enumerable() = default;

	virtual void LoadFromINI(CCINIClass *pINI) {}

	char Name[32];
};
