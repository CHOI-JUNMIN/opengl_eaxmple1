#include "context.h"
#include "image.h"
#include <imgui.h>
#include "model.h"

void Context::CreateGrid(int gridSize, float boxLength)
{
    m_gridVertices.clear(); // 기존 그리드 데이터를 초기화
    m_boldGridVertices.clear(); // 굵은 그리드
    m_gridColors.clear();       // 일반 그리드 색상
    m_boldGridColors.clear();   // 굵은 그리드 색상

    glm::vec3 boldColor(0.6f, 0.6f, 0.6f); // 굵은 그리드 색상 
    glm::vec3 gridColor(0.9f, 0.9f, 0.9f); // 일반 그리드 색상 
    glm::vec3 xAxisColor(0.0f, 0.0f, 1.0f); // x=0 색상 
    glm::vec3 zAxisColor(1.0f, 0.0f, 0.0f); // z=0 색상

    for (int i = -gridSize; i <= gridSize; i++)
    {
        // x=0 또는 z=0 축을 굵게 처리 (별도의 굵은 그리드)
        if (i == 0)
        {
            // x=0 (파란색, 굵은 선)
            m_boldGridVertices.push_back(i * boxLength);
            m_boldGridVertices.push_back(0.0f);
            m_boldGridVertices.push_back(-gridSize * boxLength);

            m_boldGridVertices.push_back(i * boxLength);
            m_boldGridVertices.push_back(0.0f);
            m_boldGridVertices.push_back(gridSize * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_boldGridColors.push_back(xAxisColor.r);
                m_boldGridColors.push_back(xAxisColor.g);
                m_boldGridColors.push_back(xAxisColor.b);
            }

            // z=0 (빨간색, 굵은 선)
            m_boldGridVertices.push_back(-gridSize * boxLength);
            m_boldGridVertices.push_back(0.0f);
            m_boldGridVertices.push_back(i * boxLength);

            m_boldGridVertices.push_back(gridSize * boxLength);
            m_boldGridVertices.push_back(0.0f);
            m_boldGridVertices.push_back(i * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_boldGridColors.push_back(zAxisColor.r);
                m_boldGridColors.push_back(zAxisColor.g);
                m_boldGridColors.push_back(zAxisColor.b);
            }
        }
        else if (i % 5 == 0) // 5의 배수는 일반 그리드로 처리하지만 색상은 다르게
        {
            // 일반 그리드로 5의 배수 표시 (boldColor 사용, 굵기 동일)
            m_gridVertices.push_back(i * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(-gridSize * boxLength);

            m_gridVertices.push_back(i * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(gridSize * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_gridColors.push_back(boldColor.r); // 5의 배수 색상 (회색)
                m_gridColors.push_back(boldColor.g);
                m_gridColors.push_back(boldColor.b);
            }

            m_gridVertices.push_back(-gridSize * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(i * boxLength);

            m_gridVertices.push_back(gridSize * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(i * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_gridColors.push_back(boldColor.r); // 5의 배수 색상 (회색)
                m_gridColors.push_back(boldColor.g);
                m_gridColors.push_back(boldColor.b);
            }
        }
        else // 일반 그리드
        {
            m_gridVertices.push_back(i * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(-gridSize * boxLength);

            m_gridVertices.push_back(i * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(gridSize * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_gridColors.push_back(gridColor.r); // 일반 그리드 색상
                m_gridColors.push_back(gridColor.g);
                m_gridColors.push_back(gridColor.b);
            }

            m_gridVertices.push_back(-gridSize * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(i * boxLength);

            m_gridVertices.push_back(gridSize * boxLength);
            m_gridVertices.push_back(0.0f);
            m_gridVertices.push_back(i * boxLength);

            for (int j = 0; j < 2; j++)
            {
                m_gridColors.push_back(gridColor.r); // 일반 그리드 색상
                m_gridColors.push_back(gridColor.g);
                m_gridColors.push_back(gridColor.b);
            }
        }
    }
}

void Context::RenderGrid(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &gridTransform)
{
    m_program1->Use(); // 셰이더 프로그램 사용

    // 그리드의 변환 행렬을 별도로 적용
    auto transform = projection * view * gridTransform;

    m_program1->SetUniform("transform", transform);
    // 1. 일반 그리드 그리기
    glLineWidth(1.0f); // 일반 그리드와 5의 배수는 기본 굵기
    glBindVertexArray(m_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridVertices.size() * sizeof(float), m_gridVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // 일반 그리드 색상 적용
    glBindBuffer(GL_ARRAY_BUFFER, m_gridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridColors.size() * sizeof(float), m_gridColors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_gridVertices.size() / 3));

    // 2. 굵은 그리드 그리기 (x=0, z=0인 축만 굵게)
    glLineWidth(1.5f); // x=0, z=0 축만 굵게 설정
    glBindVertexArray(m_boldGridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridVertices.size() * sizeof(float), m_boldGridVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // 굵은 그리드 색상 적용
    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridColors.size() * sizeof(float), m_boldGridColors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_boldGridVertices.size() / 3));
}

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::Reshape(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void Context::ProcessInput(GLFWwindow *window)
{
    if (!m_cameraControl)
        return;
    const float cameraSpeed = 0.15f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraRight;

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::MouseMove(double x, double y)
{
    if (!m_cameraControl)
        return;
    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 1.0f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f)
        m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f)
        m_cameraYaw -= 360.0f;
    if (m_cameraPitch > 89.0f)
        m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f)
        m_cameraPitch = -89.0f;

    m_prevMousePos = pos;
}
void Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            // 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
            m_prevMousePos = glm::vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_cameraControl = false;
        }
    }
}
bool Context::Init()
{
    m_model = Model::Load("./model/crate1.3ds");
    if (!m_model)
        return false;

    m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    if (!m_simpleProgram)
        return false;

    m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    if (!m_program)
        return false;

    m_program1 = Program::Create("./shader/grid.vs", "./shader/grid.fs");
    if (!m_program)
        return false;

    int gridSize = 50;      // 그리드 크기
    float boxLength = 1.0f; // 각 그리드의 크기
    CreateGrid(gridSize, boxLength);

    glGenVertexArrays(1, &m_gridVAO);
    glGenBuffers(1, &m_gridVBO);
    glGenBuffers(1, &m_gridColorVBO);

    glBindVertexArray(m_gridVAO);

    // 그리드 버텍스 설정
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridVertices.size() * sizeof(float), m_gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 그리드 색상 설정
    glBindBuffer(GL_ARRAY_BUFFER, m_gridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridColors.size() * sizeof(float), m_gridColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 굵은 그리드 설정
    glGenVertexArrays(1, &m_boldGridVAO);
    glGenBuffers(1, &m_boldGridVBO);
    glGenBuffers(1, &m_boldGridColorVBO);

    glBindVertexArray(m_boldGridVAO);

    // 굵은 그리드 버텍스 설정
    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridVertices.size() * sizeof(float), m_boldGridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 굵은 그리드 색상 설정
    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridColors.size() * sizeof(float), m_boldGridColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return true;
}

void Context::Render()
{
    if (ImGui::Begin("ui window"))
    {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
        {
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera"))
        {
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
        ImGui::Separator();
        ImGui::DragFloat("a", &a, 0.1f, -5.0f, 5.0f);
        //ImGui::Checkbox("animation", &m_animation);
    }
    ImGui::End();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_cameraFront =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // 맨뒤에 1이면 점, 0이면 벡터 0을 집어넣으면 평행이동이 안됨

    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 100.0f);

    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    glm::mat4 gridTransform = glm::mat4(1.0f);
    RenderGrid(view, projection, gridTransform);

    m_program->Use();

    glm::mat4 modelTransform = glm::mat4(1.0f);
    auto transform = projection * view * modelTransform;

    glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_program->SetUniform("modelscale", modelMatrix);

    m_model->SetPosition(a);

    m_model->Draw(m_program.get());
}