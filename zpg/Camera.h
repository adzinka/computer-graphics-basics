// Camera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

class ShaderProgram;

class Camera {
public:
    // Konstruktor nastaví výchozí hodnoty
    Camera();

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    // ... stávající metody ...
    void move(Camera_Movement direction, float deltaTime);

    // Metody pro získání finálních matic, které se pošlou do shaderu
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec3& getPosition() const { return position_; }

    void addObserver(ShaderProgram* observer);
    void removeObserver(ShaderProgram* observer);

    // Upravíme metody, které mění stav, aby volaly notify()
    void onMouseMove(float xoffset, float yoffset);
    void onWindowResize(float width, float height);
    void setPosition(const glm::vec3& pos);

private:
    // Pomocná metoda pro přepočet vektorů po změně úhlů
    void updateCameraVectors();
    void notify() const;

    // --- Vlastnosti pohledové matice ---
    glm::vec3 position_; // Pozice kamery ve světě
    glm::vec3 front_;    // Vektor, kam se kamera dívá
    glm::vec3 up_;       // Vektor "nahoru" z pohledu kamery
    glm::vec3 right_;    // Vektor "doprava" z pohledu kamery
    glm::vec3 worldUp_;  // Globální "nahoru" vektor (obvykle 0,1,0)

    // Eulerovy úhly pro orientaci kamery
    float yaw_;   // Otáčení doleva/doprava
    float pitch_; // Otáčení nahoru/dolů

    // --- Vlastnosti projekční matice ---
    float fov_;          // Zorné pole (Field of View) ve stupních
    float aspectRatio_;  // Poměr stran okna
    float nearPlane_;    // Blízká ořezávací rovina
    float farPlane_;     // Vzdálená ořezávací rovina

    std::vector<ShaderProgram*> observers_;
};