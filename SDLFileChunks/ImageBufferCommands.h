#pragma once
#include <vector>
#include <string>
#include <cstring>

#include "Command.h"
#include "Level.h"
#include "FileChunk.h"
#include "Asset.h"
#include "ImageBufferState.h"
#include "ImageBufferUtils.h"

inline size_t GetChunkOffsetBytes(Level* level, int chunkIndex)
{
    const auto& chunks = level->GetFileChunks();
    size_t off = 0;
    for (int i = 0; i < chunkIndex; i++)
    {
        const Asset* a = chunks[i]->GetChunk();
        off += (size_t)a->GetDataSize();
    }
    return off;
}

inline size_t GetTotalBytes(Level* level)
{
    const auto& chunks = level->GetFileChunks();
    size_t total = 0;
    for (auto* fc : chunks)
    {
        const Asset* a = fc->GetChunk();
        total += (size_t)a->GetDataSize();
    }
    return total;
}


class CreateImageBufferCommand : public Command
{
public:
    CreateImageBufferCommand(Level* lvl, ImageBufferState* st, std::string outName = "NewImage.tga")
        : level(lvl), state(st), outFile(std::move(outName)) {
    }

    void Execute() override
    {
        if (!level || !state) return;

        const size_t total = GetTotalBytes(level);
        state->buffer.assign(total, 0);
        state->currentChunkIndex = 0;

        SaveBufferToFile(outFile, state->buffer);
    }

    void Undo() override
    {
        if (!state) return;

        state->Reset();
        SaveBufferToFile(outFile, state->buffer);
    }

    bool IsRedoable() const override { return false; }

private:
    Level* level;
    ImageBufferState* state;
    std::string outFile;
};


class DeleteImageBufferCommand : public Command
{
public:
    DeleteImageBufferCommand(ImageBufferState* st, std::string outName = "NewImage.tga")
        : state(st), outFile(std::move(outName)) {
    }

    void Execute() override
    {
        if (!state) return;

        backup = state->buffer;
        backupIndex = state->currentChunkIndex;

        state->Reset();
        SaveBufferToFile(outFile, state->buffer);
    }

    void Undo() override
    {
        if (!state) return;

        state->buffer = backup;
        state->currentChunkIndex = backupIndex;

        SaveBufferToFile(outFile, state->buffer);
    }

    bool IsRedoable() const override { return false; }

private:
    ImageBufferState* state;
    std::string outFile;

    std::vector<unsigned char> backup;
    int backupIndex = 0;
};


class AddChunkCommand : public Command
{
public:
    AddChunkCommand(Level* lvl, ImageBufferState* st, std::string outName = "NewImage.tga")
        : level(lvl), state(st), outFile(std::move(outName)) {
    }

    void Execute() override
    {
        if (!level || !state) return;
        if (state->buffer.empty()) return;

        const auto& chunks = level->GetFileChunks();
        if (state->currentChunkIndex < 0 || state->currentChunkIndex >= (int)chunks.size())
            return;

        executedIndex = state->currentChunkIndex;

        const Asset* asset = chunks[executedIndex]->GetChunk();
        const size_t offset = GetChunkOffsetBytes(level, executedIndex);
        const size_t size = (size_t)asset->GetDataSize();

        if (offset + size > state->buffer.size()) return;

        std::memcpy(state->buffer.data() + offset, asset->GetData(), size);

        state->currentChunkIndex++;

        SaveBufferToFile(outFile, state->buffer);
    }

    void Undo() override
    {
        if (!level || !state) return;
        if (state->buffer.empty()) return;
        if (executedIndex < 0) return;

        const auto& chunks = level->GetFileChunks();
        Asset* asset = chunks[executedIndex]->GetChunk();

        const size_t offset = GetChunkOffsetBytes(level, executedIndex);
        const size_t size = (size_t)asset->GetDataSize();

        if (offset + size > state->buffer.size()) return;

        std::memset(state->buffer.data() + offset, 0, size);
        state->currentChunkIndex--;

        SaveBufferToFile(outFile, state->buffer);
    }

private:
    Level* level;
    ImageBufferState* state;
    std::string outFile;

    int executedIndex = -1;
};


class RemoveChunkCommand : public Command
{
public:
    RemoveChunkCommand(Level* lvl, ImageBufferState* st, std::string outName = "BigFile.tga")
        : level(lvl), state(st), outFile(std::move(outName)) {
    }

    void Execute() override
    {
        if (!level || !state) return;
        if (state->buffer.empty()) return;

        const auto& chunks = level->GetFileChunks();
        if (state->currentChunkIndex <= 0) return;

        targetIndex = state->currentChunkIndex - 1;
        if (targetIndex < 0 || targetIndex >= (int)chunks.size()) return;

        const Asset* asset = chunks[targetIndex]->GetChunk();

        const size_t offset = GetChunkOffsetBytes(level, targetIndex);
        const size_t size = (size_t)asset->GetDataSize();
        if (offset + size > state->buffer.size()) return;

        removedBytes.assign(state->buffer.begin() + offset, state->buffer.begin() + offset + size);

        std::memset(state->buffer.data() + offset, 0, size);
        state->currentChunkIndex--;

        SaveBufferToFile(outFile, state->buffer);
    }

    void Undo() override
    {
        if (!level || !state) return;
        if (state->buffer.empty()) return;
        if (targetIndex < 0) return;

        const auto& chunks = level->GetFileChunks();
        Asset* asset = chunks[targetIndex]->GetChunk();

        const size_t offset = GetChunkOffsetBytes(level, targetIndex);
        const size_t size = (size_t)asset->GetDataSize();
        if (offset + size > state->buffer.size()) return;
        if (removedBytes.size() != size) return;

        std::memcpy(state->buffer.data() + offset, removedBytes.data(), size);
        state->currentChunkIndex++;

        SaveBufferToFile(outFile, state->buffer);
    }

private:
    Level* level;
    ImageBufferState* state;
    std::string outFile;

    int targetIndex = -1;
    std::vector<unsigned char> removedBytes;
};
