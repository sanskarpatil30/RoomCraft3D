#include "save_load.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::string typeToString(FurnitureType type) {
    switch (type) {
        case FurnitureType::BED:        return "BED";
        case FurnitureType::CHAIR:      return "CHAIR";
        case FurnitureType::TABLE:      return "TABLE";
        case FurnitureType::SOFA:       return "SOFA";
        case FurnitureType::WARDROBE:   return "WARDROBE";
        case FurnitureType::FLOOR_LAMP: return "FLOOR_LAMP";
    }
    return "UNKNOWN";
}

static FurnitureType stringToType(const std::string& str) {
    if (str == "BED")        return FurnitureType::BED;
    if (str == "CHAIR")      return FurnitureType::CHAIR;
    if (str == "TABLE")      return FurnitureType::TABLE;
    if (str == "SOFA")       return FurnitureType::SOFA;
    if (str == "WARDROBE")   return FurnitureType::WARDROBE;
    if (str == "FLOOR_LAMP") return FurnitureType::FLOOR_LAMP;
    return FurnitureType::TABLE;
}

bool SaveLoad::saveToFile(const std::string& filepath, const Scene& scene, const Room& room) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        std::cerr << "[Error] Could not open file for writing: " << filepath << std::endl;
        return false;
    }

    out << "# RoomCraft 3D Layout File\n";
    out << "# Format: Architectural layout and furniture specifications\n\n";

    // Room properties
    out << "ROOM_DIMENSIONS " << room.width << " " << room.height << " " << room.depth << "\n";
    out << "FLOOR_COLOR " << room.floorColor.r << " " << room.floorColor.g << " " << room.floorColor.b << " " << room.floorColor.a << "\n";
    out << "WALL_COLOR "  << room.wallColor.r  << " " << room.wallColor.g  << " " << room.wallColor.b  << " " << room.wallColor.a  << "\n\n";

    // Furniture properties
    out << "FURNITURE_COUNT " << scene.items.size() << "\n";
    for (const auto& item : scene.items) {
        out << "ITEM " << typeToString(item.type) << " "
            << item.posX << " " << item.posY << " " << item.posZ << " "
            << item.rotY << " "
            << item.scaleX << " " << item.scaleY << " " << item.scaleZ << " "
            << item.primaryColor.r << " " << item.primaryColor.g << " "
            << item.primaryColor.b << " " << item.primaryColor.a << " "
            << item.secondaryColor.r << " " << item.secondaryColor.g << " "
            << item.secondaryColor.b << " " << item.secondaryColor.a << "\n";
    }

    out.close();
    std::cout << "[Save] Scene successfully saved to: " << filepath << std::endl;
    return true;
}

bool SaveLoad::loadFromFile(const std::string& filepath, Scene& scene, Room& room) {
    std::ifstream in(filepath);
    if (!in.is_open()) {
        std::cerr << "[Error] Could not open file for reading: " << filepath << std::endl;
        return false;
    }

    scene.items.clear();
    scene.selectedIndex = -1;
    scene.nextId = 1;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "ROOM_DIMENSIONS") {
            iss >> room.width >> room.height >> room.depth;
        } else if (command == "FLOOR_COLOR") {
            iss >> room.floorColor.r >> room.floorColor.g >> room.floorColor.b >> room.floorColor.a;
        } else if (command == "WALL_COLOR") {
            iss >> room.wallColor.r >> room.wallColor.g >> room.wallColor.b >> room.wallColor.a;
        } else if (command == "ITEM") {
            std::string typeStr;
            float px, py, pz, rot, sx, sy, sz;
            float pr, pg, pb, pa, sr, sg, sb, sa;

            iss >> typeStr >> px >> py >> pz >> rot >> sx >> sy >> sz
                >> pr >> pg >> pb >> pa >> sr >> sg >> sb >> sa;

            Furniture item(stringToType(typeStr), px, pz, scene.nextId++);
            item.posY = py;
            item.rotY = rot;
            item.scaleX = sx;
            item.scaleY = sy;
            item.scaleZ = sz;
            item.primaryColor = Color(pr, pg, pb, pa);
            item.secondaryColor = Color(sr, sg, sb, sa);

            scene.items.push_back(item);
        }
    }

    in.close();

    if (!scene.items.empty()) {
        scene.selectedIndex = 0;
        scene.items[0].isSelected = true;
    }

    std::cout << "[Load] Scene loaded successfully (" << scene.items.size() << " items) from: " << filepath << std::endl;
    return true;
}
