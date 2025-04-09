#pragma once

namespace Settings
{
	namespace Combat
	{
		bool Enable = true;
		bool DrawFOV = false;
		bool MuzzleLine = false;
		bool Prediction = true;
		bool PredDot = true;
		bool Triggerbot = false;
		float Smooth = 2;
		float AimFOV = 30;
		int AimbotKeybind = VK_SHIFT;
		int TriggerKeybind = VK_SHIFT;
	}

	namespace Visuals
	{
		bool Enable = true;
		bool Box = false;
		bool CornerBox = false;
		bool Filledbox = false;
		bool Skeleton = true;
		bool Name = true;
		bool Weapon;
		bool Platform = true;
		bool Rank = false;
		bool Arrows = false;
		bool Distance = true;
		bool TeamID = false;

		bool TextOutline = true;
		float MaxDistance = 300;
		double FontSize = 10.0;
		float SkeletonThickness = 1.5f;
		float BoxThickness = 1.5f;
	}

	namespace World
	{
		bool Enable = false;
		bool Pickup = false;

		float MaxDistance = 300;
		double FontSize = 10.0;
	}

	namespace Misc
	{
		bool SkipKnocked = true;
		bool SkipBot = true;
		bool VisibleCheck = true;
		bool IgnoreBots = true;
		bool IgnoreKnocked = false;
		bool IgnoreDying = true;
		FLinearColor VisibleColor = FLinearColor(0.388f, 1.0f, 0.502f, 1.0f);
		FLinearColor NotVisibleColor = FLinearColor(1.0f, 0.388f, 0.388f, 1.0f);
		FLinearColor KnockedColor = FLinearColor(1.0f, 1.0f, 0.431f, 1.0f);

	}
}