#pragma once
#include <vector>
#include "DrawableObject.h"

class Scene {
public:
    void add(DrawableObject obj) { objects_.push_back(std::move(obj)); }
    void drawAll() const {
        for (const auto& o : objects_) o.draw();
    }
private:
    std::vector<DrawableObject> objects_;
};
