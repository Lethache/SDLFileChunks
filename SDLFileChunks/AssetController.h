#pragma once

#include <map>
#include <string>

#include "StackAllocator.h"
#include "Singleton.h"

class Asset;

class AssetController : public Singleton<AssetController>
{
public:
    AssetController();
    virtual ~AssetController();

    void Initialize(int stackSize);
    void Clear();

    void ResetStack();                 
    void DeleteAsset(const std::string& guid);
    Asset* GetAsset(std::string guid);

    static StackAllocator* Stack;

private:
    std::map<std::string, Asset*> m_assets;
};
