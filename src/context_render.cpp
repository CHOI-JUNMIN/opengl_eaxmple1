#include "context.h"
#include <imgui.h>
#include <chrono>
#include "implot.h"
#include <random> // <random> 헤더 파일 포함
#include <ctime>  // 현재 시간을 시드로 설정하기 위해 사용

std::vector<int> visited_states;
float x_data[11];
float visit_counts[11] = {0};
int episode_count = 0;
int max_episodes = 500;
float V[11] = {0};
float M[500] = {0};
float reward = 0;
int count_negative = 0; 
int count_positive = 0;
//int total_moves = 0;
std::default_random_engine generator(static_cast<unsigned>(time(0)));
std::uniform_real_distribution<double> distribution(0.0, 1.0);

float alpha = 0.001f;
int a = 0;
float R = 0;
int count = 0;
void Context::Render()
{
    ImGui::SetNextWindowSize(ImVec2(400, 600)); // 임의의 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 임의의 위치 설정
    if (ImGui::Begin("ui window"))
    {
        
        while (episode_count < max_episodes)
        {
            int random_change = (distribution(generator) < 0.5) ? -1 : 1;
            a += random_change;
            if (a >= -5 && a <= 5)
            {
                int index = a + 5;
                visit_counts[index]++;
                //M[count] = index ;
                visited_states.push_back(index);
            }
            //count++;
            if (a <= -5 || a >= 5)
            {
                R= (a == 5) ? 1 : -1;

                for (int state : visited_states)
                {
                    V[state] = (1 - alpha) * V[state] + alpha * R;
                }

                if (a == -5)
                    count_negative++;
                else if (a == 5)
                    count_positive++;
                a = 0;
                //count = 0;
                episode_count++;
                visited_states.clear();
            }
        }

        ImGui::Text("Episode count: %d / %d (-5: %d, +5: %d)", episode_count, max_episodes, count_negative, count_positive);
        ImGui::Separator();
        if (ImPlot::BeginPlot("Visits graph", ImVec2(350, 250)))
        {
            ImPlot::SetupAxisLimits(ImAxis_X1, -5, 5);

            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 2000);



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
            for (int i = 0; i < 11; ++i)
            {
                x_data[i] = i - 5;
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImPlot::PlotLine("V(s)", x_data, V, 11);
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