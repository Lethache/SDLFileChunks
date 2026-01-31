#ifndef LEVEL_H
#define LEVEL_H

#include "FileChunk.h"
#include "Unit.h"
#include <vector>


    
class Level : public Resource
{
public:
	//constructor/ Deconstructor 
	Level();
	virtual ~Level();

// Existing methods...
    
	//Methods 
	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;
	
	std::vector<FileChunk*>& GetFileChunks() { return m_fileChunks; }

	//getters
	unsigned char* GetImageBuffer() { return m_imageBuffer; }
	int GetImageBufferSize() const { return m_imageBufferSize; }
	int GetCurrentChunkIndex() const { return m_currentChunkIndex; }

	void SetImageBuffer(unsigned char* p) { m_imageBuffer = p; }
	void SetImageBufferSize(int s) { m_imageBufferSize = s; }
	void SetCurrentChunkIndex(int i) { m_currentChunkIndex = i; }
private:
	//Members 
	int m_mapSizeX;
	int m_mapSizeY;
	float m_gameTime;
	vector<Unit*> m_units;
	//And other interesting level members

	//setters 
	std::vector<FileChunk*> m_fileChunks;

	unsigned char* m_imageBuffer = nullptr;
	int m_imageBufferSize = 0;
	int m_currentChunkIndex = 0;
	
};

#endif // !LEVEL_H

