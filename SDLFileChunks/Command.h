#pragma once
class Command
{
public:
	virtual ~Command() {}
	virtual void Execute() = 0;
	virtual void Undo() = 0;

	virtual bool CanRedo() const { 
		return true;
	}
};

