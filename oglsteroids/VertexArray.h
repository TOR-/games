#pragma once

class VertexArray
{
	public:
		VertexArray(const float* verts, unsigned n_verts, const unsigned* indices, unsigned n_indices);
		~VertexArray();

		// Activate vertex array ( make available for drawing )
		void SetActive();

		unsigned get_nVerts() const {  return _nVerts; }
		unsigned get_nIndices() const { return _nIndices; }
	private:
		unsigned _nVerts;
		unsigned _nIndices;
		unsigned _vertexBufferID;
		unsigned _indexBufferID;
		unsigned _vertexArrayID;
};
