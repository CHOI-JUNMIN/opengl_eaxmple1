#include "context.h"
#include <imgui.h>

static std::vector<float> values;
const int NUM_BINS = 10;
static float histogram_bins[NUM_BINS] = {0.0f};

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

        int random_change = rand() % 2 == 0 ? 1 : -1;
        a += random_change;

        if (values.size() < 1000) // 배열의 크기를 100으로 설정
            values.push_back(a);
        else
        {
            values[values_offset] = a;
            values_offset = (values_offset + 1) % values.size();
        }
        if (a < -5 || a > 5)
            a = 0;
        ImGui::DragFloat("a", &a, 0.1f, -5.0f, 5.0f);

        ImGui::PlotLines("Lines", values.data(), values.size(), values_offset, nullptr, -5.0f, 5.0f, ImVec2(0, 80));
        UpdateHistogram();
        ImGui::PlotHistogram("Histogram", histogram_bins, NUM_BINS, 0, nullptr, 0.0f, 10.0f, ImVec2(0, 80));
        // ImGui::Checkbox("animation", &m_animation);
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

    glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_program->SetUniform("modelscale", modelMatrix);

    m_model->SetPosition(a);

    m_model->Draw(m_program.get());
}

void Context::UpdateHistogram()
{
    std::fill(histogram_bins, histogram_bins + NUM_BINS, 0.0f);

    const float min_value = -5.0f;
    const float max_value = 5.0f;
    const float bin_size = (max_value - min_value) / NUM_BINS;

    for (float value : values)
    {
        int bin_index = static_cast<int>((value - min_value) / bin_size);
        bin_index = std::clamp(bin_index, 0, NUM_BINS - 1);
        histogram_bins[bin_index]++;
    }
}