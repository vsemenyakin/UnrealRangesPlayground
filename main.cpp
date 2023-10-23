#include "UnrealRanges/UnrealRanges_Common.h"
#include "TinyAsciiGameEngine/TinyAsciiGameEngine.h"

struct Base
{
	virtual ~Base() { }
};

struct Child : public Base
{
};

struct NotRelated
{
	virtual ~NotRelated() { }
};


int main()
{
	FWorld World{ 60, 25 };

	std::shared_ptr<FUnit> Unit1 = World.Add(FUnit{ { 3.5f, 5.5f } });
	Unit1->Visual = '1';

	std::shared_ptr<FUnit> Unit2 = World.Add(FUnit{ { 45.5f, 13.5f } });
	Unit2->Visual = '2';

	std::shared_ptr<FUnit> Unit3 = World.Add(FUnit{ { 17.5f, 22.5f } });
	Unit3->Visual = '3';

	std::shared_ptr<FUnit> Unit4 = World.Add(FUnit{ { 52.5f, 3.5f } });
	Unit4->Visual = '4';

	std::shared_ptr<FUnit> Unit5 = World.Add(FUnit{ { 27.5f, 14.5f } });
	Unit5->Visual = '5';

	std::shared_ptr<FUnit> Unit6 = World.Add(FUnit{ { 10.5f, 11.5f } });
	Unit6->Visual = '6';

	std::shared_ptr<FUnit> Unit7 = World.Add(FUnit{ { 53.5f, 22.5f } });
	Unit7->Visual = '7';

	std::shared_ptr<FUnit> Unit8 = World.Add(FUnit{ { 33.5f, 1.5f } });
	Unit8->Visual = '8';

	std::shared_ptr<FUnit> Unit9 = World.Add(FUnit{ { 15.5f, 4.5f } });
	Unit9->Visual = '9';

	std::shared_ptr<FCircle> Circle1 = World.Add(FCircle{ { 10.5f, 10.5f } , 8.f });

	std::shared_ptr<FCircle> Circle2 = World.Add(FCircle{ { 20.5f, 13.5f } , 10.f });

	World.Render();

	return 0;
}
