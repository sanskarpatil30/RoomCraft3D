#include "scene.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Scene::Scene() : selectedIndex(-1), nextId(1), statusTimer(0.0f) {
    statusMessage = "Welcome to RoomCraft 3D! Press [P] to load sample room.";
    loadSampleRoom();
}

void Scene::clear() {
    items.clear();
    selectedIndex = -1;
    setStatus("Room cleared. Press 1-6 to add furniture.");
}

void Scene::setStatus(const std::string& msg) {
    statusMessage = msg;
    std::cout << "[RoomCraft3D] " << msg << std::endl;
}

void Scene::loadSampleRoom() {
    items.clear();
    selectedIndex = -1;
    nextId = 1;

    // 1. King Bed placed against back wall
    Furniture bed(FurnitureType::BED, -2.4f, -2.4f, nextId++);
    bed.rotY = 0.0f;
    bed.primaryColor = Palette::SoftNavy;
    items.push_back(bed);

    // 2. Wardrobe placed along left wall
    Furniture wardrobe(FurnitureType::WARDROBE, -3.8f, 1.2f, nextId++);
    wardrobe.rotY = 90.0f;
    wardrobe.primaryColor = Palette::DarkWalnut;
    items.push_back(wardrobe);

    // 3. Lounge Sofa facing room center
    Furniture sofa(FurnitureType::SOFA, 2.6f, -1.8f, nextId++);
    sofa.rotY = -90.0f;
    sofa.primaryColor = Palette::SageGreen;
    items.push_back(sofa);

    // 4. Floor Lamp placed beside sofa
    Furniture lamp(FurnitureType::FLOOR_LAMP, 3.6f, -3.2f, nextId++);
    lamp.rotY = 0.0f;
    items.push_back(lamp);

    // 5. Dining / Workspace Table
    Furniture table(FurnitureType::TABLE, 1.8f, 2.2f, nextId++);
    table.rotY = 0.0f;
    table.primaryColor = Palette::WarmWood;
    items.push_back(table);

    // 6. Chair tucked at the table
    Furniture chair(FurnitureType::CHAIR, 1.8f, 3.0f, nextId++);
    chair.rotY = 180.0f;
    chair.primaryColor = Palette::MutedOrange;
    items.push_back(chair);

    if (!items.empty()) {
        selectedIndex = 0;
        items[0].isSelected = true;
    }

    setStatus("Sample furnished room loaded (6 items).");
}

Furniture* Scene::getSelected() {
    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        return &items[selectedIndex];
    }
    return nullptr;
}

const Furniture* Scene::getSelected() const {
    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        return &items[selectedIndex];
    }
    return nullptr;
}

void Scene::selectNone() {
    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        items[selectedIndex].isSelected = false;
    }
    selectedIndex = -1;
    setStatus("Deselected object.");
}

void Scene::selectNext() {
    if (items.empty()) return;

    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        items[selectedIndex].isSelected = false;
    }

    selectedIndex = (selectedIndex + 1) % items.size();
    items[selectedIndex].isSelected = true;

    setStatus("Selected: " + items[selectedIndex].name);
}

void Scene::selectPrev() {
    if (items.empty()) return;

    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        items[selectedIndex].isSelected = false;
    }

    selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    items[selectedIndex].isSelected = true;

    setStatus("Selected: " + items[selectedIndex].name);
}

void Scene::addFurniture(FurnitureType type, const Room& room) {
    // Unselect previous
    if (selectedIndex >= 0 && selectedIndex < (int)items.size()) {
        items[selectedIndex].isSelected = false;
    }

    // Default spawn position at center of room
    float spawnX = 0.0f;
    float spawnZ = 0.0f;

    Furniture newItem(type, spawnX, spawnZ, nextId++);
    newItem.isSelected = true;

    items.push_back(newItem);
    selectedIndex = (int)items.size() - 1;

    setStatus("Added " + newItem.name + " to room.");
}

void Scene::deleteSelected() {
    if (selectedIndex < 0 || selectedIndex >= (int)items.size()) {
        setStatus("No object selected to delete.");
        return;
    }

    std::string deletedName = items[selectedIndex].name;
    items.erase(items.begin() + selectedIndex);

    if (items.empty()) {
        selectedIndex = -1;
    } else {
        if (selectedIndex >= (int)items.size()) {
            selectedIndex = (int)items.size() - 1;
        }
        items[selectedIndex].isSelected = true;
    }

    setStatus("Deleted " + deletedName + ".");
}

void Scene::duplicateSelected(const Room& room) {
    if (selectedIndex < 0 || selectedIndex >= (int)items.size()) return;

    Furniture copy = items[selectedIndex];
    items[selectedIndex].isSelected = false;

    copy.id = nextId++;
    copy.posX += 0.8f;
    copy.posZ += 0.8f;

    // Check bounds
    float halfW, halfD, h;
    copy.getExtents(halfW, halfD, h);
    if (!room.isInside(copy.posX, copy.posZ, halfW, halfD)) {
        copy.posX -= 1.6f;
        copy.posZ -= 1.6f;
    }

    copy.isSelected = true;
    items.push_back(copy);
    selectedIndex = (int)items.size() - 1;

    setStatus("Duplicated " + copy.name + ".");
}

void Scene::moveSelected(float dx, float dz, const Room& room) {
    Furniture* f = getSelected();
    if (!f) return;

    float newX = f->posX + dx;
    float newZ = f->posZ + dz;

    float halfW, halfD, h;
    f->getExtents(halfW, halfD, h);

    // Check if new position fits within room boundaries
    if (room.isInside(newX, newZ, halfW, halfD)) {
        f->posX = newX;
        f->posZ = newZ;

        // Check for collision warning with other furniture
        if (checkCollision(selectedIndex)) {
            setStatus("Warning: " + f->name + " overlaps another object!");
        } else {
            setStatus("Moved " + f->name + ".");
        }
    } else {
        setStatus("Notice: Object reached room boundary wall.");
    }
}

void Scene::rotateSelected(float dAngle) {
    Furniture* f = getSelected();
    if (!f) return;

    f->rotY += dAngle;
    while (f->rotY >= 360.0f) f->rotY -= 360.0f;
    while (f->rotY < 0.0f) f->rotY += 360.0f;

    setStatus("Rotated " + f->name + " to " + std::to_string((int)f->rotY) + " deg.");
}

void Scene::scaleSelected(float factor) {
    Furniture* f = getSelected();
    if (!f) return;

    float newScale = f->scaleX * factor;
    if (newScale >= 0.5f && newScale <= 2.2f) {
        f->scaleX = newScale;
        f->scaleY = newScale;
        f->scaleZ = newScale;
        setStatus("Scaled " + f->name + " to " + std::to_string((int)(newScale * 100)) + "%.");
    } else {
        setStatus("Scale limit reached (50% - 220%).");
    }
}

void Scene::cycleSelectedColor() {
    Furniture* f = getSelected();
    if (!f) return;

    f->cycleColor();
    setStatus("Changed color of " + f->name + ".");
}

bool Scene::checkCollision(int index) const {
    if (index < 0 || index >= (int)items.size()) return false;

    const Furniture& a = items[index];
    float aHW, aHD, aH;
    a.getExtents(aHW, aHD, aH);

    for (size_t i = 0; i < items.size(); ++i) {
        if ((int)i == index) continue;

        const Furniture& b = items[i];
        float bHW, bHD, bH;
        b.getExtents(bHW, bHD, bH);

        // Axis-Aligned Bounding Box (AABB) 2D overlap on floor plane
        bool overlapX = std::fabs(a.posX - b.posX) < (aHW + bHW);
        bool overlapZ = std::fabs(a.posZ - b.posZ) < (aHD + bHD);

        if (overlapX && overlapZ) {
            return true;
        }
    }
    return false;
}

void Scene::render() const {
    for (const auto& item : items) {
        item.render();
    }
}
