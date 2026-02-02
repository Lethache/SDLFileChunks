#pragma once
#include <stack>
#include "Command.h"

class CommandManager
{
public:
    void Do(Command* cmd);
    void Undo();
    void Redo();
    void Clear();

    int UndoCount() const { return (int)m_undo.size(); }
    int RedoCount() const { return (int)m_redo.size(); }

    ~CommandManager();

private:
    std::stack<Command*> m_undo;
    std::stack<Command*> m_redo;
};
