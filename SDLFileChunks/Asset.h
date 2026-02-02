#ifndef  ASSET_H
#define ASSET_H

#include "StandartIncludes.h"
#include "ObjectPool.h"
typedef unsigned char Byte1;

class Asset
{ //Constructors/Destructors
public:
	Asset();
	virtual ~Asset() {}

	//Accessors
	string GetGUID() { return m_GUID; }
	void SetGUID(string _guid) { m_GUID = _guid; }
	void SetDataSize(int _size) { m_dataSize = _size; }
	void SetData(Byte1* _data) { m_data = _data; }

	int GetDataSize() const { return m_dataSize; }

	Byte1* GetData() { return m_data; }
	const Byte1* GetData() const { return m_data; }
    
    const std::string& GetGUID() const { return m_GUID; }
	//Methods 
	void Reset();
	void ToString();
	static ObjectPool<Asset>* Pool;



private:
	//Members 
	string m_GUID;
	int m_dataSize;
	Byte1* m_data;
};


#endif // ! ASSET_h

