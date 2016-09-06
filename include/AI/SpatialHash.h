#ifndef ENJON_SPATIALHASH_H
#define ENJON_SPATIALHASH_H

#include <vector>
#include <set>
#include <stdlib.h>

#include <ECS/Entity.h>
#include <Math/Maths.h>
#include <Utils/Errors.h>
#include <Physics/AABB.h>

typedef Enjon::Math::Vec2 V2; 

const int CELL_SIZE = 64;

namespace SpatialHash { 

	typedef struct 
	{
		std::vector<Enjon::uint32> entities;
		Enjon::uint32 ParentIndex;
		float ObstructionValue;
	} Cell;

	typedef struct                 
	{
		int rows;
		int cols;
		int CellSize;
		EM::Vec2 Origin;
		std::vector<Cell> cells; 
		std::vector<Enjon::uint32> dirtyCells;
	} Grid; 

	/* Inits spatial grid based on width, height of level and given cell size */ 
	void Init(Grid* grid, int width, int height, int cell_size = CELL_SIZE); 

	/* Finds particular cell that a given entity belongs to based on its position */
	EM::Vec2 FindGridCoordinates(Grid* grid, V2& position); 

	/* Overloaded function that finds particular cell that a given entity belongs to based on its AABB (preferred method) */
	std::vector<Enjon::uint32> FindCell(Grid* grid, Enjon::uint32 entity, const Enjon::Physics::AABB* AABB); 

	/* Clears all entity vectors from every cell in the spatial grid */
	void ClearCells(Grid* grid);

	/* Finds all neighboring cells to a given entitiy's cell and stores those in a passed in entities vector */
	void GetNeighborCells(Grid* grid, int index, std::vector<Enjon::uint32>* Entities);

	void DrawGrid(Grid* G, EG::SpriteBatch* Batch, EM::Vec2& Position = EM::Vec2(0.0f));

	void DrawActiveCell(Grid* G, EG::SpriteBatch* Batch, EM::Vec2& Position);

	EM::Vec4 GetCellDimensions(Grid* G, EM::Vec2& Cell);

	EM::Vec2 FindGridCoordinatesFromIndex(Grid* G, Enjon::uint32 Index);

	void FindCells(Grid* G, Enjon::uint32 Entity, const EP::AABB* AABB, EM::Vec4* CellRange, float ObstructionValue);

	std::vector<Enjon::uint32> GetEntitiesFromCells(Grid* G, EM::Vec4& Cells);

	Enjon::uint32 GetGridIndexFromCoordinates(Grid* G, EM::Vec2& Coordinates);
}

#endif