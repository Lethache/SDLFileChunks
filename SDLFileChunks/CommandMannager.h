#pragma once
#include <stack>
#include "Command.h"

class CommandMannager
{
public:
    void Do(Command* cmd)
    {
        if (cmd == nullptr) return;

        cmd->Execute();
        m_undo.push(cmd);

        while (!m_redo.empty())
        {
            delete m_redo.top();
            m_redo.pop();
        }
    }

    void Undo()
    {
        if (m_undo.empty()) return;

        Command* cmd = m_undo.top();
        m_undo.pop();

        cmd->Undo();

        if (cmd->CanRedo())
            m_redo.push(cmd);
        else
            delete cmd;
    }

    void Redo()
    {
        if (m_redo.empty()) return;

        Command* cmd = m_redo.top();
        m_redo.pop();

        cmd->Execute();
        m_undo.push(cmd);
    }

    ~CommandMannager()
    {
        while (!m_undo.empty()) { delete m_undo.top(); m_undo.pop(); }
        while (!m_redo.empty()) { delete m_redo.top(); m_redo.pop(); }
    }

private:
    std::stack<Command*> m_undo;
    std::stack<Command*> m_redo;
};

