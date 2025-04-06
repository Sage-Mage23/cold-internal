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
		bool Triggerbot = true;
		float Smooth = 2;
		float AimFOV = 30;
		int Keybind = VK_SHIFT;
		int Bind = 0;
	}

	namespace Visuals
	{
		bool Enable = true;
		bool Box = false;
		bool filled_box = false;
		bool Skeleton = true;
		bool Name = true;
		bool Weapon;
		bool Platform = true;
		bool Rank = false;
		bool Distance = true;
		bool view_line = false;
		bool oof = false;

		bool TextOutline = true;
		float MaxDistance = 300;
		double FontSize = 10.0;
	}

	namespace Misc
	{
		bool SkipKnocked = true;
		bool SkipBot = true;
		bool VisibleCheck = true;

		FLinearColor VisibleColor = FLinearColor(0.388f, 1.0f, 0.502f, 1.0f);
		FLinearColor NotVisibleColor = FLinearColor(1.0f, 0.388f, 0.388f, 1.0f);
		FLinearColor KnockedColor = FLinearColor(1.0f, 1.0f, 0.431f, 1.0f);

	}
}