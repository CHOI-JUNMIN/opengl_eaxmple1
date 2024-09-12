#include "context.h"

void Context::CreateGrid(int gridSize, float boxLength)
{
    m_gridVertices.clear();
    m_boldGridVertices.clear();
    m_gridColors.clear();
    m_boldGridColors.clear();

    int totalGridVertices = (gridSize * 2 + 1) * 4;
    m_gridVertices.reserve(totalGridVertices * 3);
    m_boldGridVertices.reserve(12);
    m_gridColors.reserve(totalGridVertices * 3);
    m_boldGridColors.reserve(12);

    glm::vec3 boldColor(0.6f, 0.6f, 0.6f);
    glm::vec3 gridColor(0.9f, 0.9f, 0.9f);
    glm::vec3 xAxisColor(0.0f, 0.0f, 1.0f);
    glm::vec3 zAxisColor(1.0f, 0.0f, 0.0f);

    for (int i = -gridSize; i <= gridSize; i++)
    {
        if (i == 0)
        {
            AddBoldGridLine(i, -gridSize * boxLength, gridSize * boxLength, xAxisColor, zAxisColor);
        }
        else if (i % 5 == 0)
        {
            AddGridLine(i, -gridSize * boxLength, gridSize * boxLength, boldColor);
        }
        else
        {
            AddGridLine(i, -gridSize * boxLength, gridSize * boxLength, gridColor);
        }
    }
}

void Context::AddGridLine(float pos, float start, float end, const glm::vec3 &color)
{
    // X축 선
    m_gridVertices.push_back(pos);
    m_gridVertices.push_back(0.0f);
    m_gridVertices.push_back(start);
    m_gridVertices.push_back(pos);
    m_gridVertices.push_back(0.0f);
    m_gridVertices.push_back(end);

    m_gridColors.push_back(color.r);
    m_gridColors.push_back(color.g);
    m_gridColors.push_back(color.b);
    m_gridColors.push_back(color.r);
    m_gridColors.push_back(color.g);
    m_gridColors.push_back(color.b);

    // Z축 선
    m_gridVertices.push_back(start);
    m_gridVertices.push_back(0.0f);
    m_gridVertices.push_back(pos);
    m_gridVertices.push_back(end);
    m_gridVertices.push_back(0.0f);
    m_gridVertices.push_back(pos);

    m_gridColors.push_back(color.r);
    m_gridColors.push_back(color.g);
    m_gridColors.push_back(color.b);
    m_gridColors.push_back(color.r);
    m_gridColors.push_back(color.g);
    m_gridColors.push_back(color.b);
}

void Context::AddBoldGridLine(float pos, float start, float end, const glm::vec3 &xColor, const glm::vec3 &zColor)
{
    m_boldGridVertices.push_back(pos);
    m_boldGridVertices.push_back(0.0f);
    m_boldGridVertices.push_back(start);
    m_boldGridVertices.push_back(pos);
    m_boldGridVertices.push_back(0.0f);
    m_boldGridVertices.push_back(end);

    m_boldGridColors.push_back(xColor.r);
    m_boldGridColors.push_back(xColor.g);
    m_boldGridColors.push_back(xColor.b);
    m_boldGridColors.push_back(xColor.r);
    m_boldGridColors.push_back(xColor.g);
    m_boldGridColors.push_back(xColor.b);

    m_boldGridVertices.push_back(start);
    m_boldGridVertices.push_back(0.0f);
    m_boldGridVertices.push_back(pos);
    m_boldGridVertices.push_back(end);
    m_boldGridVertices.push_back(0.0f);
    m_boldGridVertices.push_back(pos);

    m_boldGridColors.push_back(zColor.r);
    m_boldGridColors.push_back(zColor.g);
    m_boldGridColors.push_back(zColor.b);
    m_boldGridColors.push_back(zColor.r);
    m_boldGridColors.push_back(zColor.g);
    m_boldGridColors.push_back(zColor.b);
}

void Context::RenderGrid(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &gridTransform)
{
    m_program1->Use();
    auto transform = projection * view * gridTransform;
    m_program1->SetUniform("transform", transform);

    glLineWidth(1.0f);
    glBindVertexArray(m_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridVertices.size() * sizeof(float), m_gridVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_gridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_gridColors.size() * sizeof(float), m_gridColors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_gridVertices.size() / 3));

    glLineWidth(1.5f);
    glBindVertexArray(m_boldGridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridVertices.size() * sizeof(float), m_boldGridVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_boldGridColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boldGridColors.size() * sizeof(float), m_boldGridColors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_boldGridVertices.size() / 3));
}