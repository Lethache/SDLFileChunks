#include "AssembleChunksCommand.h"
#include <fstream>
#include <cstdio> // remove 
#include <iostream>

AssembleChunksCommand::AssembleChunksCommand(Level* level, const std::string& outFile)
	:m_level(level), m_outFile(outFile) { }

void AssembleChunksCommand::Execute()
{
	std::ofstream out(m_outFile, std::ios::binary | std::ios::out);
	if (!out.is_open())
	{
		return;
	}

	auto& chunks = m_level->GetFileChunks(); 

	for (int i = 0; i < (int)chunks.size(); i++)
	{
		Asset* a = chunks[i]->GetChunk();
		out.write(reinterpret_cast<char*>(a->GetData()), a->GetDataSize());
	}

	out.close();
	std::cout << "Assembled: " << m_outFile << std::endl;
}

void AssembleChunksCommand::Undo()
{
	std::remove(m_outFile.c_str());
	std::cout << "Deleted: " << m_outFile << std::endl;

}