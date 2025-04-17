#include <revid_engine/renderer/vulkan/Mesh.h>

#include <revid_engine/ServiceLocater.h>

Revid::Mesh::Mesh(String file_name)
{
	// Vertex portions
	Vector<Maths::Vec3> vertex_positions;
	Vector<Maths::Vec2> vertex_texcoords;
	Vector<Maths::Vec3> vertex_normals;

	// Face vectors
	std::vector<uint32_t> vertex_position_indicies;
	std::vector<uint32_t> vertex_texcoord_indicies;
	std::vector<uint32_t> vertex_normal_indicies;

	std::stringstream ss;
	// std::ifstream in_file("./assets/obj/grass.obj");
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	Maths::Vec3 temp_vec3;
	Maths::Vec2 temp_vec2;
	uint32_t temp_glint = 0;

	// File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	// Read one line at a time
	while (std::getline(in_file, line))
	{
		// Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		// Vertex position
		if (prefix == "v")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		// Vertex texture
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		// Vertex normal
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		// Face
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_glint)
			{
				// Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_glint);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(temp_glint);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_glint);

				// Handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					counter = 0;
					ss.ignore(1, ' ');
				}

				// Reset the counter
				if (counter > 2)
				{
					counter = 0;
				}
			}
		}
	}

	m_vertices.resize(vertex_position_indicies.size(), SimpleVertex());
	m_indices.resize(vertex_position_indicies.size(), uint32_t(0));

	int size = m_vertices.size();
	static int j = 0;
	for (size_t i = 0; i < size; i++)
	{
		m_vertices[i].m_position = vertex_positions[vertex_position_indicies[i] - 1];
		m_vertices[i].m_normal = vertex_normals[vertex_normal_indicies[i] - 1];
		m_vertices[i].m_color = Maths::Vec3(j, (j+1)/2.0, (j+2)/3.0);
		m_indices[i] = i;
	}
	j++;

	m_instanceCount = 1;
	m_modelMatrix = glm::mat4(1.0f);
	m_device = ServiceLocator::GetRenderer()->GetDeivce();
	createVertexBuffer();
	createIndexBuffer();
}


void Revid::Mesh::createVertexBuffer()
{
	VmaAllocation vertexAlloc;
	ServiceLocator::GetRenderer()->createGenericBuffer(
		m_vertices,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
		m_vertexBuffer,
		vertexAlloc);
}

void Revid::Mesh::createIndexBuffer()
{
	VmaAllocation vertexAlloc;
	ServiceLocator::GetRenderer()->createGenericBuffer(
		m_indices,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
		m_indexBuffer,
		vertexAlloc);
}