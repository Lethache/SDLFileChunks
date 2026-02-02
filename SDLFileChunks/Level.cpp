#include "Level.h"
#include "FileChunk.h"
#include "AssetController.h"


Level::Level()
{
	
		AssetController::Instance().Initialize(1024 * 1024 * 64);

		if (Unit::Pool == nullptr)       Unit::Pool = new ObjectPool<Unit>();
		if (SoundEffect::Pool == nullptr) SoundEffect::Pool = new ObjectPool<SoundEffect>();
		if (FileChunk::Pool == nullptr)  FileChunk::Pool = new ObjectPool<FileChunk>();

		m_mapSizeX = 0;
		m_mapSizeY = 0;
		m_gameTime = 0.0f;

		m_units.clear();
		m_fileChunks.clear();
	


}

Level::~Level()
{
	
	for (auto* fc : m_fileChunks)
	{
		if (fc != nullptr)
			FileChunk::Pool->ReleaseResource(fc);
	}
	m_fileChunks.clear();
	
	delete FileChunk::Pool;
	FileChunk::Pool = nullptr;
	AssetController::Instance().Clear();

	
	delete SoundEffect::Pool;
	SoundEffect::Pool = nullptr;

	delete Unit::Pool;
	Unit::Pool = nullptr;

	m_units.clear();
}

void Level::AssignNonDefaultValues() {
	m_mapSizeX = 128;
	m_mapSizeY = 256;
	m_gameTime = 101.5f;

	
	for (auto* fc : m_fileChunks)
		FileChunk::Pool->ReleaseResource(fc);
	m_fileChunks.clear();
	for (int i = 0; i < 7; i++)
	{
		std::string name = "chunk" + std::to_string(i) + ".bin";
		FileChunk* fc = FileChunk::Pool->GetResource();
		fc->SetChunk(AssetController::Instance().GetAsset(name)); 
		m_fileChunks.push_back(fc);
	}
	m_units.clear();
}

void Level::Serialize(std::ostream& out)
{
	out.write(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	out.write(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	out.write(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));

	int chunkCount = (int)m_fileChunks.size();
	out.write(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

	
	for (int i = 0; i < chunkCount; i++)
	{
		std::string guid = m_fileChunks[i]->GetChunk()->GetGUID(); 
		int len = (int)guid.size();
		out.write(reinterpret_cast<char*>(&len), sizeof(len));
		out.write(guid.c_str(), len);
	}
}

void Level::Deserialize(std::istream& in)
{
	in.read(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	in.read(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	in.read(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));

	
	for (auto* fc : m_fileChunks)
		FileChunk::Pool->ReleaseResource(fc);
	m_fileChunks.clear();

	int chunkCount = 0;
	in.read(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

	for (int i = 0; i < chunkCount; i++)
	{
		int len = 0;
		in.read(reinterpret_cast<char*>(&len), sizeof(len));

		std::string guid(len, '\0');
		in.read(&guid[0], len);

		FileChunk* fc = FileChunk::Pool->GetResource();
		fc->SetChunk(AssetController::Instance().GetAsset(guid)); 
		m_fileChunks.push_back(fc);
	}
}


void Level::ToString()
{
	cout << "LEVEL" << endl;
	cout << "MapSizeX: " << m_mapSizeX << endl;
	cout << "MapSizeY: " << m_mapSizeY << endl;
	cout << "GameTime: " << m_gameTime << endl;
	for (int count = 0; count < m_units.size(); count++) {
		m_units[count]->ToString();
	}
	Resource::ToString();


	cout << "FileChunks: " << m_fileChunks.size() << endl;
	for (int i = 0; i < (int)m_fileChunks.size(); i++)
	{
		cout << i << ") " << m_fileChunks[i]->GetChunk()->GetGUID()
			<< " size= " << m_fileChunks[i]->GetChunk()->GetDataSize() << endl;
	}
}
void Level::ClearFileChunksAndAssets()
{
	for (auto* fc : m_fileChunks)
	{
		if (fc && fc->GetChunk())
		{
			AssetController::Instance().DeleteAsset(fc->GetChunk()->GetGUID());
		}

		if (fc)
			FileChunk::Pool->ReleaseResource(fc);
	}

	m_fileChunks.clear();
}