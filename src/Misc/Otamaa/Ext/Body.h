#pragma once

#include <YRPP.h>
#include <ArrayClasses.h>

// All ext classes should derive from this class
#define FAIL_CHECK(hr) if(FAILED(hr)) return hr;

class IExtension
{
public:
	virtual ~IExtension() { };

	virtual size_t GetSize() const = 0;
	virtual HRESULT Load(IStream* pStm, AbstractClass* pOwner) = 0;
	virtual HRESULT Save(IStream* pStm) = 0;
};

template<typename T>
class TExtension : public IExtension
{
	T* AttachedToObject;

public:
	TExtension(T* const OwnerObject) : IExtension()
		, AttachedToObject(OwnerObject)
	{
	}

	virtual ~TExtension() { };

	T* const& OwnerObject() const
	{ return this->AttachedToObject; }

	void SetOwnerObject(T* const& pOwner)
	{ AttachedToObject = pOwner; }

};

// This class is just a wrapper to replace
// interact with AbstractClass

class ExtensionWrapper
{
public:
	ExtensionWrapper()
	{
		this->FlagDirty = FALSE;
		this->ExtensionObject = nullptr;
	}

	~ExtensionWrapper()
	{
		if (this->ExtensionObject)
			GameDelete(this->ExtensionObject);
	}

	size_t GetSize() const
	{
		if (this->ExtensionObject)
			return this->ExtensionObject->GetSize();

		return 0;
	}

	inline static ExtensionWrapper*& GetWrapper(void* pThis)
	{
		return *reinterpret_cast<ExtensionWrapper**>((int)pThis + 0x20);
	}

	inline void DestoryExtensionObject()
	{
		GameDelete(this->ExtensionObject);
		this->ExtensionObject = nullptr;
	}

	template <typename TExt, typename AttachedTo>
	inline void CreateExtensionObject(AttachedTo* OwnerObject)
	{
		this->DestoryExtensionObject();
		this->ExtensionObject = GameCreate<TExt>(OwnerObject);
	}

	HRESULT Load(IStream* pStm, AbstractClass* pOwner) const
	{
		if (auto pExtData = this->ExtensionObject)
		{
			HRESULT hr = pStm->Read(pExtData, pExtData->GetSize(), nullptr);

			if (SUCCEEDED(hr))
				return pExtData->Load(pStm, pOwner);

			return hr;
		}

		return S_OK;
	}

	HRESULT Save(IStream* pStm) const
	{
		if (auto pExtData = this->ExtensionObject)
		{
			HRESULT hr = pStm->Write(pExtData, pExtData->GetSize(), nullptr);

			if (SUCCEEDED(hr))
				return pExtData->Save(pStm);

			return hr;
		}

		return S_OK;
	}

	bool IsDirty() const
	{
		return this->FlagDirty;
	};

	inline void SetDirtyFlag(bool fDirty)
	{
		this->FlagDirty = fDirty;
	}

private:
	bool FlagDirty;

public:
	IExtension* ExtensionObject {};
};

template<class T>
class HelperClass : public T
{
public:
	inline static HelperClass* GetHelperClass(T* pSrc)
	{
		return static_cast<HelperClass*>(pSrc);
	}

	template<typename TAs>
	inline TAs* As() const
	{
		return (TAs*)this;
	}

	template<typename TAs>
	inline HelperClass<TAs>* AsHelper() const
	{
		return (HelperClass<TAs>*)this;
	}
};

#define READ(what)\
	pStm->Read(&what, sizeof(what), nullptr);

#define WRITE(what)\
	pStm->Write(&what, sizeof(what), nullptr);

// Westwood style save & load helper
// Westwood like such kind of things

namespace SaveLoadHelpers
{
	template<typename T>
	HRESULT Save(IStream* pStm, const DynamicVectorClass<T>& data);

	template<typename T>
	HRESULT Load(IStream* pStm, DynamicVectorClass<T>& data);

	template<typename T>
	HRESULT Swizzle(IStream* pStm, const DynamicVectorClass<T>& data);

	template<typename T>
	HRESULT Save(IStream* pStm, const TypeList<T>& data);

	template<typename T>
	HRESULT Load(IStream* pStm, TypeList<T>& data);
}