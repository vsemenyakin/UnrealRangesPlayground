#include "TinyAsciiGameEngine.h"

#include <iostream>
#include <vector>
#include <memory>

//#define USE_RANGES
#define USE_ALGO

// ---------------------------------------------------

FVector2D operator+(const FVector2D& A, const FVector2D& B)
{
	return { A.X + B.X, A.Y + B.Y };
}

FVector2D operator-(const FVector2D& A, const FVector2D& B)
{
	return { A.X - B.X, A.Y - B.Y };
}

bool IsPointInRect(const FVector2D& Point, const FVector2D& RectMin, const FVector2D& RectMax)
{
	return (Point.X >= RectMin.X && Point.X < RectMax.X) && (Point.Y >= RectMin.Y && Point.Y < RectMax.Y);
}

bool IsPointInCircle(const FVector2D& Center, const float Radius, const FVector2D& Point)
{
	const float Radius2 = Radius * Radius;
	const FVector2D PointFromCircleCenter = Point - Center;

	const float PointFromCircleCenterDistance2 =
		PointFromCircleCenter.X * PointFromCircleCenter.X +
		PointFromCircleCenter.Y * PointFromCircleCenter.Y;

	return (PointFromCircleCenterDistance2 <= Radius2);
}

// ---------------------------------------------------

FUnit::FUnit(const FVector2D& InPotision)
	: Position(InPotision) { }

FCircle::FCircle(const FVector2D& InCenter, const float InRadius)
	: Center(InCenter), Radius(InRadius) { }

// ---------------------------------------------------

FWorld::FWorld(const int32_t InMaxCellX, const int32_t InMaxCellY)
	: MaxCellX{ InMaxCellX }, MaxCellY{ InMaxCellY } { }
	
void FWorld::Render()
{
	for (int32_t CellY = 0; CellY <= MaxCellY; ++CellY)
	{
		for (int32_t CellX = 0; CellX <= MaxCellX; ++CellX)
		{
			std::cout << GetCellSymbol(CellX, CellY);
		}

		std::cout << '|';
		std::cout << std::endl;
	}

	for (int32_t CellX = 0; CellX <= MaxCellX; ++CellX)
	{
		std::cout << '-';
	}
}

std::shared_ptr<FUnit> FWorld::Add(FUnit&& Unit)
{
	auto NewUnit = std::make_shared<FUnit>(std::move(Unit));
	Units.emplace_back(NewUnit);
	
	return NewUnit;
}

std::shared_ptr<FCircle> FWorld::Add(FCircle&& Circle)
{
	auto NewCircle = std::make_shared<FCircle>(std::move(Circle));
	Circlies.emplace_back(NewCircle);
	
	return NewCircle;
}

char FWorld::GetCellSymbol(int32_t CellX, int32_t CellY) const
{
	const FVector2D CellRectMin = { CellX * CellSideSize , CellY * CellSideSize };
	const FVector2D CellRectMax = CellRectMin + CellSizeVector;

	if (const FUnit* UnitInCell = FindFirstUnitInRect(CellRectMin, CellRectMax))
	{
		return UnitInCell->Visual;
	}
	else if (const FCircle* Circle = FindFirstCircleThatSideIsInRect(CellRectMin, CellRectMax))
	{
		return Circle->Visual;
	}
	else
	{
		return ' ';
	}
}

const FUnit* FWorld::FindFirstUnitInRect(const FVector2D& RectMin, const FVector2D& RectMax) const
{
	using namespace WorldLocal;

	auto IsUnitInRect = [&RectMin, &RectMax](const FUnit* Unit)
	{
		return IsPointInRect(Unit->Position, RectMin, RectMax);
	};

	//================================================================================================== >>>

#	if defined(USE_RANGES) //[1] Ranges

	return ToPtr(GetUnitsRange(), IsUnitInRect));

#	elif defined(USE_ALGO) //[2] Only Algo, without Ranges

	auto IsValidatedUnitInRect = [&IsUnitInRect](const std::weak_ptr<FUnit>& PossibleInvalidUnit)
	{
		if (PossibleInvalidUnit.expired())
		{
			return false;
		}

		return IsUnitInRect(&*PossibleInvalidUnit.lock());
	};

	return ToPtr(FindByPredicate(Units, IsValidatedUnitInRect));

#	else //[3] Raw STL, without any utils

	for (std::weak_ptr<FUnit> Unit : Units)
	{
		if (Unit.expired())
		{
			continue;
		}
	
		std::shared_ptr<FUnit> UnitLocked = Unit.lock();

		if (IsUnitInRect(UnitLocked))
		{
			return &*UnitLocked;
		}
	}
	
	return nullptr;

#	endif
	//================================================================================================== <<<
}

const FCircle* FWorld::FindFirstCircleThatSideIsInRect(const FVector2D& RectMin, const FVector2D& RectMax) const
{
	using namespace WorldLocal;

	auto IsCircleSideInRect = [&RectMin, &RectMax](const FCircle* Circle)
	{
		int32_t CornersInCircle = 0;
		CornersInCircle += IsPointInCircle(Circle->Center, Circle->Radius, RectMin) ? 1 : 0;
		CornersInCircle += IsPointInCircle(Circle->Center, Circle->Radius, { RectMax.X, RectMin.Y }) ? 1 : 0;
		CornersInCircle += IsPointInCircle(Circle->Center, Circle->Radius, RectMax) ? 1 : 0;
		CornersInCircle += IsPointInCircle(Circle->Center, Circle->Radius, { RectMin.X, RectMax.Y }) ? 1 : 0;

		return CornersInCircle > 0 && CornersInCircle < 4;
	};

	return ToPtr(FindByPredicate(GetCircliesRange(), IsCircleSideInRect));
}
