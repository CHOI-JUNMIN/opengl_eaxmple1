#include "context.h"
#include <imgui.h>
#include <chrono>
#include "implot.h"

static std::chrono::time_point<std::chrono::steady_clock> last_update_time;
static float update_interval = 0.01f; // 0.1초마다 값 업데이트

std::vector<int> visited_states;
float x_data[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
float visit_counts[11] = {0};
int episode_count = 0;
int max_episodes = 100;
float V[11] = {0};
float reward = 0;
int count_negative = 0; 
int count_positive = 0;

int total_moves = 0; // 총 이동 횟수 추적

void Context::Render()
{
    ImGui::SetNextWindowSize(ImVec2(400, 600)); // 임의의 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 임의의 위치 설정
    if (ImGui::Begin("ui window"))
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - last_update_time;

        if (episode_count < max_episodes && elapsed.count() > update_interval) // 설정한 간격보다 시간이 지나면 업데이트
        {

            int random_change = rand() % 2 == 0 ? 1 : -1;
            a += random_change;
            last_update_time = now; // 업데이트된 시간을 기록

            if (a >= -5 && a <= 5)
            {
                int index = a + 5;               
                visit_counts[index]++;           
                visited_states.push_back(index); 
            }

            // a가 -6 또는 6에 도달하면 에피소드 종료
            if (a <= -6 || a >= 6)
            {
                // 보상 계산 ([-5, -1]일 때 -1 보상, [1, 5]일 때 1 보상)
                reward = (a == 6) ? 1 : (a == -6) ? -1: 0;

                // 지나간 모든 상태에 대해 보상을 할당 
                for (int state : visited_states)
                {
                    V[state] += reward; // 해당 상태의 기댓값을 보상으로 업데이트
                }

                visited_states.clear(); // 방문한 상태 기록 초기화

                if (a == -6)
                    count_negative++;
                else if (a == 6)
                    count_positive++;
                a = 0;           
                episode_count++; 
            }
        }
        ImGui::Text("Episode count: %d / %d (-5: %d, +5: %d)", episode_count, max_episodes, count_negative, count_positive);

        ImGui::Separator();
        if (ImPlot::BeginPlot("Visits graph", ImVec2(350, 250)))
        {
            ImPlot::SetupAxisLimits(ImAxis_X1, -5, 5);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 800);

            ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImPlot::PlotLine("Visits", x_data, visit_counts, 11);
            ImPlot::PopStyleColor();
            ImPlot::EndPlot();
        }

        ImGui::Separator();
        if (ImPlot::BeginPlot("Expectation of Return", ImVec2(350, 250)))
        {
            ImPlot::SetupAxisLimits(ImAxis_X1, -5, 5);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);

            float x_data[11];
            float v_data[11];
            for (int i = 0; i < 11; ++i)
            {
                x_data[i] = i - 5;                                                 
                v_data[i] = (visit_counts[i] > 0) ? V[i] / visit_counts[i] : 0.0f; // 기댓값 V(s) 계산 (방문 횟수로 나눈 값)
            }

            ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImPlot::PlotLine("V(s)", x_data, v_data, 11);
            ImPlot::PopStyleColor();
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