#include <iostream>
#include <fstream>
#include <cctype>

#include "Level.h"
#include "FileChunk.h"
#include "CommandManager.h"
#include "AssembleChunksCommand.h"
#include "AssetController.h"

#include "ImageBufferState.h"
#include "ImageBufferCommands.h"

using namespace std;

static void PrintMenu(int chunkIndex, const CommandManager& mgr)
{
    cout
        << "\n[Q]uit [S]ave Level [L]oad Level [Z] Undo [Y] Redo\n"
        << "[C]reate image buffer  [D]elete image buffer  [A]dd chunk  [R]emove chunk\n"
        << "[B]uild/Rebuild tga (Assemble)\n"
        << "Index (" << chunkIndex << ")  Undo count (" << mgr.UndoCount()
        << ")  Redo count (" << mgr.RedoCount() << ")\n";
}

int main()
{
   
    AssetController::Instance().Initialize(1024 * 1024 * 64);

    if (Unit::Pool == nullptr)        Unit::Pool = new ObjectPool<Unit>();
    if (SoundEffect::Pool == nullptr) SoundEffect::Pool = new ObjectPool<SoundEffect>();
    if (FileChunk::Pool == nullptr)   FileChunk::Pool = new ObjectPool<FileChunk>();

    Level* level = new Level();
    level->AssignNonDefaultValues();

    CommandManager mgr;
    ImageBufferState img;

    char input = 0;

    while (true)
    {
        PrintMenu(img.currentChunkIndex, mgr);

        cin >> input;
        input = (char)tolower((unsigned char)input);

        if (input == 'q')
            break;

        switch (input)
        {
        case 's':
        {
            ofstream out("level.bin", ios::binary);
            if (!out.is_open())
            {
                cout << "Cannot open level.bin for writing.\n";
                break;
            }

            level->Serialize(out);
            out.close();

            cout << "\nLevel Saved:";
            level->ToString();
            break;
        }

        case 'l':
        {
            
            mgr.Clear();
            img.Reset();

        
            level->ClearFileChunksAndAssets();

            
            AssetController::Instance().Clear();

          
            ifstream in("level.bin", ios::binary);
            if (!in.is_open())
            {
                cout << "Cannot open level.bin for reading.\n";
                break;
            }

            level->Deserialize(in);
            in.close();

            cout << "\nLoaded Level:";
            level->ToString();
            break;
        }

       
     
        case 'c':
        {
            mgr.Do(new CreateImageBufferCommand(level, &img, "NewImage.tga"));
            cout << "\nCreated image buffer -> NewImage.tga\n";
            break;
        }

        case 'd':
        {
            mgr.Do(new DeleteImageBufferCommand(&img, "NewImage.tga"));
            cout << "\nDeleted image buffer -> NewImage.tga\n";
            break;
        }

        case 'a':
        {
            mgr.Do(new AddChunkCommand(level, &img, "NewImage.tga"));
            cout << "\nAdded chunk -> NewImage.tga\n";
            break;
        }

        case 'r':
        {
            mgr.Do(new RemoveChunkCommand(level, &img, "NewImage.tga"));
            cout << "\nRemoved chunk -> NewImage.tga\n";
            break;
        }

        case 'z':
        {
            mgr.Undo();
            cout << "\nUndo\n";
            break;
        }

        case 'y':
{
    if (mgr.RedoCount() == 0)
        cout << "\nNothing to redo\n";
    else
    {
        mgr.Redo();
        cout << "\nRedo\n";
    }
    break;
}

        default:
            cout << "Unknown command.\n";
            break;
        }
    }

    mgr.Clear();
    delete level;

    
    FileChunk::Pool = nullptr;
    delete FileChunk::Pool; 
    SoundEffect::Pool = nullptr;
    delete SoundEffect::Pool;
    Unit::Pool = nullptr;
    delete Unit::Pool;      
    

    return 0;
}
