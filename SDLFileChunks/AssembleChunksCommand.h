#pragma once 
#include "Command.h"
#include "Level.h"
#include <string>

class AssembleChunksCommand : public Command
{
public:
	AssembleChunksCommand(Level* level, const std::string& outFile);
	void Execute() override;
	void Undo() override; 


private :
	Level* m_level = nullptr;
	std::string m_outFile;
};