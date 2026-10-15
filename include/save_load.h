#pragma once
#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <string>
#include "scene.h"
#include "room.h"

class SaveLoad {
public:
    // Save scene and room data to a text file
    static bool saveToFile(const std::string& filepath, const Scene& scene, const Room& room);

    // Load scene and room data from a text file
    static bool loadFromFile(const std::string& filepath, Scene& scene, Room& room);
};

#endif // SAVE_LOAD_H
