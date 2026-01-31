#include "FileChunk.h"
#include "Asset.h"
ObjectPool<FileChunk>* FileChunk::Pool = nullptr;

FileChunk::FileChunk() : Resource() {}
FileChunk::~FileChunk() {}

void FileChunk::Serialize (std::ostream& _stream) {}
void FileChunk::Deserialize(std::istream& _stream) {}

void FileChunk::ToString()
{
	std::cout << "FILE CHUNK\n";
	if (m_chunk) std::cout << "Asset GIUD:" << m_chunk->GetGUID() << "\n";

}

void FileChunk::AssignmentNonDefaultValues() {

}
void FileChunk::Reset() {
	m_chunk = nullptr;
}