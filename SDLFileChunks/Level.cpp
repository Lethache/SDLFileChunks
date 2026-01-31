#include "Level.h"
#include "FileChunk.h"
#include "AssetController.h"
Level::Level()
{
	AssetController::Instance().Initialize(1024* 1024 * 64); //allocate 64mb 
	Unit::Pool = new ObjectPool<Unit>();
	SoundEffect::Pool = new ObjectPool<SoundEffect>();
	FileChunk::Pool = new ObjectPool<FileChunk>();
	m_mapSizeX = 0;
	m_mapSizeY = 0;
	m_gameTime = 0.0f;
	m_units.clear();

}

Level::~Level()
{
	m_units.clear();
	delete SoundEffect::Pool;
	delete Unit::Pool;
	AssetController::Instance().Clear(); // free 10mb 

	for (auto* fc : m_fileChunks)
		FileChunk::Pool->ReleaseResource(fc);
		m_fileChunks.clear();
		
		delete FileChunk::Pool;
}

void Level::AssignNonDefaultValues() {
	m_mapSizeX = 128;
	m_mapSizeY = 256;
	m_gameTime = 101.5f;
	for (int count = 0; count < 5; count++)
	{
		Unit* unit = Unit::Pool->GetResource();
		unit->AssignNonDefaultValues();
		m_units.push_back(unit);
	}
	for (auto* fc : m_fileChunks)
		FileChunk::Pool->ReleaseResource(fc);
	m_fileChunks.clear();

	for (int i = 0; i < 7; i++)
	{
		FileChunk* fc = FileChunk::Pool->GetResource();

		std::string filename = "chunk" + std::to_string(i) + ".bin";

		Asset* a = AssetController::Instance().GetAsset(filename);

		fc->SetChunk(a);
		m_fileChunks.push_back(fc);
	}
}

void Level::Serialize(std::ostream& _stream)
{
	_stream.write(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.write(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.write(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));

	int numberOfUnits = m_units.size();
	_stream.write(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));
	for (int count = 0; count < numberOfUnits; count++)
	{
		SerializePointer(_stream, m_units[count]);
	}

	int chunkCount = (int)m_fileChunks.size();
	_stream.write(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));
	for (int i = 0; i < chunkCount; i++)
	{
		SerializeAsset(_stream, m_fileChunks[i]->GetChunk());

	}


	Resource::Serialize(_stream);
}

void Level::Deserialize(std::istream& _stream)
{
	_stream.read(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.read(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.read(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));
	int numberOfUnits;
	_stream.read(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));

	for (int count = 0; count < numberOfUnits; count++)
	{
		Unit* unit;
		DeserializePointer(_stream, unit);
		m_units.push_back(unit);
	}
	int chunkCount = 0;
	_stream.read(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

	for (int i = 0; i < chunkCount; i++)
	{
		FileChunk* fc = FileChunk::Pool->GetResource();

		Asset* a = nullptr;
		DeserializeAsset(_stream, a);

		fc->SetChunk(a);
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