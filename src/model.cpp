#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// 텍스처있는3d모델전용
/*
void Model::Draw(const Program* program) const  //모델 그리기
{
    for (auto &mesh : m_meshes)
    {
        mesh->Draw(program);
    }
}
*/
void Model::SetPosition(float newPos)
{
    Pos = newPos;
}

void Model::Draw(const Program *program) const
{
    for (const auto &mesh : m_meshes)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        //  메쉬에 연결된 재질을 가져옴
        auto material = mesh->GetMaterial();
        // 셰이더 프로그램에 재질 정보 전달
        glm::vec3 lightPos(0.0f, 1.0f, 1.0f);

        glm::mat NewModelMatrix = glm::translate(modelMatrix, glm::vec3(Pos, 0.5f, 0.0f));

        program->Use();
        program->SetUniform("materialdiffuse", material->diffuseColor);
        program->SetUniform("materialambient", material->ambientColor);
        program->SetUniform("materialspecular", material->specularColor);
        program->SetUniform("materialshininess", material->shininess);

        program->SetUniform("lightDiffuse", glm::vec3(1.0f, 1.0f, 1.0f));  // 확산광 색상
        program->SetUniform("lightSpecular", glm::vec3(0.0f, 0.0f, 0.0f)); // 반사광 색상
        program->SetUniform("Lightambient", glm::vec3(0.3f, 0.3f, 0.3f));  // 주변광
        program->SetUniform("lightPosition", lightPos);

        program->SetUniform("modelMatrix", NewModelMatrix);

        mesh->Draw(program);
    }
}