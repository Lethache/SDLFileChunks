#ifndef FILECHUNK_H
#define FILECHUNK_H

#include "Resource.h"

class Asset;
class FileChunk : public Resource
{
public:
	//Constructor/Deconstructor
	FileChunk();
	virtual ~FileChunk();

	//Accesor 
	Asset* GetChunk() { return m_chunk; }
	void SetChunk(Asset* _part) { m_chunk = _part; }
	void Reset();
	//Methods 
	virtual void Serialize(std::ostream& _stream);
	virtual void Deserialize(std::istream& _stream);
	virtual void ToString();
	virtual void AssignmentNonDefaultValues();

	static ObjectPool<FileChunk>* Pool;

private:
	Asset* m_chunk;


};

#endif //FILECHUNK_H
