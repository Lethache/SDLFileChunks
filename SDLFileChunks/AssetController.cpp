#include "AssetController.h"
#include "FileController.h"
#include "Asset.h"
#include "ObjectPool.h"
#include <iostream>

StackAllocator* AssetController::Stack = nullptr;

AssetController::AssetController() {}
AssetController::~AssetController()
{
    Clear();
}

void AssetController::Initialize(int stackSize)
{
    if (Stack == nullptr)
        Stack = new StackAllocator();

    Stack->AllocateStack(stackSize);

    if (Asset::Pool == nullptr)
        Asset::Pool = new ObjectPool<Asset>();
}

void AssetController::ResetStack()
{
    if (Stack != nullptr)
        Stack->ClearMemory(); 
}

void AssetController::DeleteAsset(const std::string& guid)
{
    auto it = m_assets.find(guid);
    if (it == m_assets.end()) return;

    std::cout << "Deleting asset " << guid << std::endl;

    if (Asset::Pool != nullptr && it->second != nullptr)
        Asset::Pool->ReleaseResource(it->second);

    m_assets.erase(it);
}

void AssetController::Clear()
{
    if (Asset::Pool != nullptr)
    {
        for (auto const& x : m_assets)
            Asset::Pool->ReleaseResource(x.second);
    }

    m_assets.clear();

    
    ResetStack();

}

Asset* AssetController::GetAsset(std::string guid)
{
    auto it = m_assets.find(guid);
    if (it != m_assets.end())
        return it->second;

   
    if (Asset::Pool == nullptr)
        Asset::Pool = new ObjectPool<Asset>();

    Asset* asset = Asset::Pool->GetResource();

    std::cout << "Allocating asset " << guid << std::endl;

    asset->SetGUID(guid);

    auto size = FileController::Instance().GetFileSize(guid);
    asset->SetDataSize(size);

    unsigned char* mem = Stack->GetMemory(size);  
    asset->SetData(mem);

    FileController::Instance().ReadFile(guid, asset->GetData(), asset->GetDataSize());

    m_assets[guid] = asset;
    return asset;
}
