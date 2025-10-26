#pragma once

class Camera;

class ICameraObserver {
public:
    virtual ~ICameraObserver() = default;
    virtual void updateCamera(const Camera& camera) = 0;
};