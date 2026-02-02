#include "CommandManager.h"

void CommandManager::Do(Command* cmd)
{
    cmd->Execute();
    m_undo.push(cmd);

    
    while (!m_redo.empty())
    {
        delete m_redo.top();
        m_redo.pop();
    }
}

void CommandManager::Undo()
{
    if (m_undo.empty()) return;

    Command* cmd = m_undo.top();
    m_undo.pop();

    cmd->Undo();

    if (cmd->IsRedoable())
        m_redo.push(cmd);
    else
        delete cmd;
}

void CommandManager::Redo()
{
    if (m_redo.empty()) return;

    Command* cmd = m_redo.top();
    m_redo.pop();

    cmd->Execute();
    m_undo.push(cmd);
}

void CommandManager::Clear()
{
    while (!m_undo.empty()) { delete m_undo.top(); m_undo.pop(); }
    while (!m_redo.empty()) { delete m_redo.top(); m_redo.pop(); }
}

CommandManager::~CommandManager()
{
    Clear();
}
