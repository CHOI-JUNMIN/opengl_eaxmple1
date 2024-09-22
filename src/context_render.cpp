#include "context.h"
#include <imgui.h>
#include <chrono>
#include "implot.h"

static std::vector<float> values;
const int NUM_BINS = 10;
static float histogram_bins[NUM_BINS] = {0.0f};
static std::chrono::time_point<std::chrono::steady_clock> last_update_time;
static float update_interval = 0.01f; // 0.1초마다 값 업데이트
float x_data[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
float visit_counts[11] = {0};
int episode_count = 0;
int max_episodes = 100;

void Context::Render()
{
    ImGui::SetNextWindowSize(ImVec2(400, 400)); // 임의의 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 임의의 위치 설정
    if (ImGui::Begin("ui window"))
    {

        // if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
        // {
        //     glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        // }
        // ImGui::Separator();
        // ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        // ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        // ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        // ImGui::Separator();
        // if (ImGui::Button("reset camera"))
        // {
        //     m_cameraYaw = 0.0f;
        //     m_cameraPitch = 0.0f;
        //     m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        // }
        // ImGui::Separator();

        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - last_update_time;

        if (episode_count < max_episodes && elapsed.count() > update_interval) // 설정한 간격보다 시간이 지나면 업데이트
        {

            int random_change = rand() % 2 == 0 ? 1 : -1;
            a += random_change;
            last_update_time = now; // 업데이트된 시간을 기록

            for (int j = 0; j < 11; ++j)
            {
                visit_counts[j] = static_cast<float>(visit_counts[j]);
            }

            int index = a + 5;
            visit_counts[index]++;
            if (a <= -5 || a >= 5){
                a = 0;
                episode_count++;
            }
        }
        ImGui::Text("Episode count: %d", episode_count);
        if (ImPlot::BeginPlot("graph", "Coordinate", "Visits Count"))
        {
            // x축 범위 설정 (-5 ~ 5)
            ImPlot::SetupAxisLimits(ImAxis_X1, -5, 5);

            // y축 범위 설정 (0 ~ 500, 방문 횟수에 따라 조정)
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 500);

            // 선형 그래프 그리기 (PlotLine 사용)
            ImPlot::PlotLine("Visits", x_data, visit_counts, 11);

            ImPlot::EndPlot();
        }
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

// void Context::UpdateHistogram()
// {
//     std::fill(histogram_bins, histogram_bins + NUM_BINS, 0.0f);

//     const float min_value = -5.0f;
//     const float max_value = 5.0f;
//     const float bin_size = (max_value - min_value) / NUM_BINS;

//     for (float value : values)
//     {
//         int bin_index = static_cast<int>((value - min_value) / bin_size);
//         bin_index = std::clamp(bin_index, 0, NUM_BINS - 1);
//         histogram_bins[bin_index]++;
//     }
// }