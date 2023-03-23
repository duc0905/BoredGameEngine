#pragma once
#include "../pch.h"
#include "Component.h"

struct TransformComponent :
    public Component
{
    glm::vec3 translation_ = glm::vec3(0.0f), rotation_ = glm::vec3(0.0f), scale_ = glm::vec3(1.0f);
    glm::vec3 const GetTranslation() const { return translation_; };
    void Translate(const glm::vec3& translation) { translation_ += translation; }
    void SetTranslation(const glm::vec3& translation) { translation_ = translation; }

    glm::vec3 const GetRotation() const { return rotation_; };
    void Rotate(const glm::vec3& rotation) { rotation_ += rotation; }
    void SetRotation(const glm::vec3& rotation) { rotation_ = rotation; }

    glm::vec3 const GetScale() const { return scale_; };
    void Scale(const glm::vec3& scale) { scale_ *= scale; }
    void SetScale(const glm::vec3& scale) { scale_ = scale; }

    glm::mat4 const GetModelMatrix() const
    {
        glm::mat4 Model(1.0f);

        Model = glm::translate(Model, translation_);

        //Model = glm::rotate(Model, 2.0f * glm::pi<float>(), rotation_);

        Model = glm::rotate(Model, rotation_.x, {1.0f, 0.0f, 0.0f});
        Model = glm::rotate(Model, rotation_.y, {0.0f, 1.0f, 0.0f});
        Model = glm::rotate(Model, rotation_.z, {0.0f, 0.0f, 1.0f});

        Model = glm::scale(Model, scale_);

        return Model;
    }

    // Inherited via Component
    virtual const std::string& GetComponentName() override;
};

