#include <GL/glew.h>

#include "VertexArray.h"

VertexArray::VertexArray
	(const float* verts, unsigned n_verts, const unsigned* indices, unsigned n_indices)
	: _nVerts(n_verts), _nIndices(n_indices)
{
	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);
	
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, _nVerts * 3 * sizeof(float), verts, GL_STATIC_DRAW);
	
	glGenBuffers(1, &_indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nIndices * sizeof(unsigned), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &_vertexBufferID);
	glDeleteBuffers(1, &_indexBufferID);
	glDeleteVertexArrays(1, &_vertexArrayID);
}

void VertexArray::SetActive()
{
	glBindVertexArray(_vertexArrayID);
}
