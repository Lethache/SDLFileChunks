#include "Level.h"
#include "FileChunk.h"
#include "CommandMannager.h"
#include "AssembleChunksCommand.h"
int main()
{
    if (FileChunk::Pool == nullptr)
        FileChunk::Pool = new ObjectPool<FileChunk>(); 
    Level* level = new Level();
    level->AssignNonDefaultValues();

    CommandMannager mgr;
    mgr.Do(new AssembleChunksCommand(level, "Rebuilt.tga")); 
    mgr.Undo();                                             
    mgr.Redo();

    ofstream writeStream("level.bin", ios::out | ios::binary);
    level->Serialize(writeStream);
    writeStream.close();

    cout << "Level To Save:";
    level->ToString();
    delete level;
    cout << endl;

    Level* loadedLevel = new Level();
    ifstream readStream("level.bin", ios::in | ios::binary);
    loadedLevel->Deserialize(readStream);
    readStream.close();

    cout << "Loaded Level:";
    loadedLevel->ToString();
    delete loadedLevel;

}