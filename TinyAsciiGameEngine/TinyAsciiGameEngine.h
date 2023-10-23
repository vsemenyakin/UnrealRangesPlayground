#pragma once

#include "UnrealRanges_Common.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>

struct FVector2D
{
	friend FVector2D operator+(const FVector2D& A, const FVector2D& B);
	friend FVector2D operator-(const FVector2D& A, const FVector2D& B);

	float X = 0.f;
	float Y = 0.f;
};

bool IsPointInRect(const FVector2D& Point, const FVector2D& RectMin, const FVector2D& RectMax);
bool IsPointInCircle(const FVector2D& Center, const float Radius, const FVector2D& Point);

struct FUnit
{
	FUnit() = default;
	FUnit(const FVector2D& InPotision);

	FVector2D Position{ 0.f, 0.f };
	
	char Visual = '@';

	uint32_t HitPoints = 100;
	uint8_t TeamId = 0;
};

struct FCircle
{
	FCircle() = default;
	FCircle(const FVector2D& InCenter, const float InRadius);

	FVector2D Center{ 0.f, 0.f };
	float Radius{ 0.f };

	char Visual = '.';
};

// ---------------------------------------------------

class FWorld
{
public:
	FWorld(const int32_t InMaxCellX, const int32_t InMaxCellY);
	
	void Render();

	std::shared_ptr<FUnit> Add(FUnit&& Unit);
	std::shared_ptr<FCircle> Add(FCircle&& Circle);

	auto GetUnitsRange() const; //Returns "Range<FUnit*>"
	auto GetCircliesRange() const; //Returns "Range<FCircle*>"

private:
	constexpr static float CellSideSize = 1.f;
	constexpr static FVector2D CellSizeVector{ CellSideSize, CellSideSize };

	char GetCellSymbol(int32_t CellX, int32_t CellY) const;

	const FUnit* FindFirstUnitInRect(const FVector2D& RectMin, const FVector2D& RectMax) const;
	const FCircle* FindFirstCircleThatSideIsInRect(const FVector2D& RectMin, const FVector2D& RectMax) const;

	int32_t MaxCellX = 0;
	int32_t MaxCellY = 0;

	std::vector<std::weak_ptr<FUnit>> Units;
	std::vector<std::weak_ptr<FCircle>> Circlies;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace WorldLocal
{
	auto ValidWeaksRange()
	{
		//"auto" <=> "std::weak_ptr<{AnyType}>"
		return FilteredRange([](auto WeakPtr)
		{
			return !WeakPtr.expired();
		});
	}

	auto LocksRange()
	{
		//"auto" <=> "std::weak_ptr<{AnyType}>"
		//returns: "std::shared_ptr<{AnyType}>"
		return TransformedRange([](auto WeakPtr)
		{
			return WeakPtr.lock();
		});
	}

	auto ValidLocksRange()
	{
		return ValidWeaksRange() | LocksRange();
	}

	auto ToPtrRange()
	{
		//"auto" <=> thing that may be "ToPtr"-ed
		return TransformedRange([](auto ToPtrable)
		{
			return ToPtr(ToPtrable);
		});
	}
}

auto FWorld::GetUnitsRange() const
{
	using namespace WorldLocal;

	return Units | ValidLocksRange() | ToPtrRange();
}

auto FWorld::GetCircliesRange() const
{
	using namespace WorldLocal;

	return Circlies | ValidLocksRange() | ToPtrRange();
}
