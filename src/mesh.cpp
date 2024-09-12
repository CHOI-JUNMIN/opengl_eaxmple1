#include "mesh.h"

void Mesh::SetNodeName(const std::string &name)
{
    m_nodeName = name;
}

const std::string &Mesh::GetNodeName() const
{
    return m_nodeName;
}