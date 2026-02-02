#pragma once

class Command
{
public:
    virtual ~Command() = default;

    virtual void Execute() = 0;
    virtual void Undo() = 0;


    virtual bool IsRedoable() const { return true; }
};
