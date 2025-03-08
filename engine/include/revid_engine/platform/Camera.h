#pragma once
#include "maths/Vec.h"

namespace Revid
{
    class EditorCamera
    {
    private:
        int m_ViewportWidth = 1920;
        int m_ViewportHeight = 1080;

        Maths::Vec3 m_Position = Maths::Vec3(2.0f, 2.0f, 2.0f);
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        float m_FOV = 60.0f;
        float m_NearCip = 0.1f;
        float m_FarClip = 50000000.0f;
        float m_AspectRatio = 1.7778f; // 16:9 = m_ViewportWidth / m_ViewportHeight

        Maths::Vec3 m_Front = Maths::Vec3(0.0f, 0.0f, 1.0f);
        Maths::Vec3 m_Up = Maths::Vec3(0.0f, 1.0f, 0.0f);
        Maths::Vec3 m_Right = Maths::Vec3(1.0f, 0.0f, 0.0f);

        // We'll never roll the camera. https://sidvind.com/wiki/Yaw,_pitch,_roll_camera
        float m_Yaw = 0.0f;
        float m_Pitch = 0.0f;

        float m_MovementSpeed = 0.15f;
        float m_MouseSensitivity = 0.08f;
        float m_ZoomSensitivity = 0.08f;

        void updateCameraVectors();
        void updateProjectionMatrix();
        void updateViewMatrix();

    public:
        EditorCamera();
        EditorCamera(float fov, float aspect, float near, float far);

        glm::vec3 GetPosition() { return m_Position.ToGLM(); }
        glm::mat4 GetViewMatrix() { return m_ViewMatrix; };
        glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; };

        void SetPosition(const Maths::Vec3& position) { m_Position = position; };
        void SetPerspective(const glm::mat4& perspective) { m_ProjectionMatrix = perspective; };
        void SetViewMatrix(const glm::mat4& view) { m_ViewMatrix = view; };

        float GetYaw() { return m_Yaw; }
        float GetPitch() { return m_Pitch; }
        float GetMovementSpeed() { return m_MovementSpeed; }
        float GetMouseSensitivity() { return m_MouseSensitivity; }
        float GetZoom() { return m_ZoomSensitivity; }

        void SetPerspective(float fov, float aspect, float near, float far);
        void SetViewportSize(int width, int height);

        void MoveForward();
        void MoveBackward();
        void MoveLeft();
        void MoveRight();
        void MoveUp();
        void MoveDown();
        void ProcessMouseScroll(float scrollOffset);
        void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch);
    };
}

