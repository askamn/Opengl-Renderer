#pragma once
#include "Core.h"
#include "OpenGL/OpenGl.h"
namespace cm
{
	struct Vertex
	{
		Vec3 position		= Vec3(0);
		Vec3 normal			= Vec3(0);
		Vec3 tanget			= Vec3(0);
		Vec3 bitanget		= Vec3(0);
		Vec3 colour			= Vec3(0);
		Vec3 texture_coord	= Vec3(0);
	};

	struct Edge
	{
		Vertex vertices[2] = {};
		real32 length = 0;
	};

	struct Triangle
	{
		real32 area;
		Vec3 normal;
		Vertex vertices[3];
		Edge edges[2];
	};

	class IndexedTriangle
	{
	public:
		union
		{
			uint32 index[3];
			struct
			{
				uint32 v1;
				uint32 v2;
				uint32 v3;
			};
		};
		union
		{
			Vec3 edges[3];
			struct
			{
				Vec3 edge1;
				Vec3 edge2;
				Vec3 edge3;
			};
		};
		Vec3 normal;
	public:
		IndexedTriangle() {}
		~IndexedTriangle() {}		
		inline real GetArea() { return Mag(Cross(edge1, edge2)) / 2; }
		inline void CalculateNormal() { normal = Normalize(Cross(edge1, edge2)); }
	};


	


	class EditableMesh
	{
	public:
		DynaArray<Vertex> vertices;
		DynaArray<uint32> indices;
		DynaArray<IndexedTriangle> tris;

		bool has_positions			= false;
		bool has_normals			= false;
		bool has_texture_coords		= false;
		bool has_tanget_bitangets	= false;
		bool has_colours			= false;
		bool has_faces				= false;

	public:
		GLMesh CreateMesh(bool tangets);
		void LoadMesh(GLMesh mesh);
		void AddTrianlge(const Vec3 &pos1, const Vec3 &pos2, const Vec3 &pos3);
		void AddTextureCoords(const Vec3 &t1, const Vec3 &t2, const Vec3 &t3);
		void AddTrianlge(const Vertex &p1, const Vertex &p2, const Vertex &p3);
		void ConvertToPNT(DynaArray<float> *data, DynaArray<uint32> *index) const;
		void RecaluclateNormals();
		void FuseVertices(float tollerance = FLOATING_POINT_ERROR_PRESCION);
		
		void ConvertToOther(DynaArray<float> *data, DynaArray<uint32> *index) const; //<-- Just for testing purposes
		void SetColour(const Vec3 colour);
		
	};

}