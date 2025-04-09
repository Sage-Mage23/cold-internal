#pragma once
#include <windows.h>
#include <filesystem>


class UFunction;

class UObject
{
public:
	uint64_t GetVTable()
	{
		return *(uint64_t*)(this + 0x0); // UObject::vtable
	}

	EObjectFlags GetObjectFlags()
	{
		return *(EObjectFlags*)(this + 0x8); // UObject::ObjectFlags
	}

	int GetInternalIndex()
	{
		return *(int*)(this + 0xC); // UObject::InternalIndex
	}

	UObject* GetClassPrivate()
	{
		return *(UObject**)(this + 0x10); // UObject::ClassPrivate
	}

	FName GetNamePrivate()
	{
		return *(FName*)(this + 0x18); // UObject::NamePrivate
	}

	UObject* GetOuterPrivate()
	{
		return *(UObject**)(this + 0x20); // UObject::OuterPrivate
	}

	void ProcessEvent(UObject* function, void* args, void* out_args = nullptr, void* stack = nullptr) {

		if (auto vft = *(void***)this)
		{
			if (function)
			{
				reinterpret_cast<void(__cdecl*)(UObject*, UObject*, void*)>(vft[Offsets::ProcessEvent])(this, function, args);
			}
		}

	}

	static UObject* StaticFindObject(UObject* Class, UObject* Outer, const wchar_t* Name, bool ExactClass)
	{
		static void* (*StaticFindObject)(UObject * Class, UObject * InOuter, const wchar_t* Name, bool ExactClass) = nullptr;
		if (!StaticFindObject)
		{
			StaticFindObject = decltype(StaticFindObject)(Module + Offsets::StaticFindObject);
		}

		if (!StaticFindObject)
		{

		}

		return reinterpret_cast<UObject*> (StaticFindObject(Class, Outer, Name, ExactClass));
	}

	template <class Type>
	static Type FindObjectSingle(const wchar_t* Name, UObject* Outer = nullptr)
	{
		return reinterpret_cast<Type>(UObject::StaticFindObject(nullptr, Outer, Name, false));
	}
};


class USoundBase : public UObject
{
public:

};

class UFont : public UObject
{
public:
	int32_t LegacyFontSize()
	{
		return *(int32_t*)(this + 0x13c);
	}
};

class UTexture : public UObject
{
public:

};

class UTexture2D : public UTexture
{
public:

};

inline UFont* MediumFont = 0;

class UCanvas : public UObject
{
public:
	float ClipX()
	{
		return *(float*)(this + 0x30);
	}

	float ClipY()
	{
		return *(float*)(this + 0x34);
	}

	UTexture2D* DefaultTexture()
	{
		return *(UTexture2D**)(this + 0x70);
	}

	void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, FLinearColor RenderColor)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawLine");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector2D ScreenPositionA;
			FVector2D ScreenPositionB;
			float Thickness;
			FLinearColor RenderColor;
		} Params = { ScreenPositionA, ScreenPositionB, Thickness, RenderColor };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawText(FString RenderText, FVector2D ScreenPosition, double FontSize, FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined)
	{
		UFont* RenderFont = MediumFont;
		*(int32*)(RenderFont + 0x13c) = FontSize;

		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawText");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} Params = { RenderFont, RenderText, ScreenPosition, FVector2D(1.0, 1.0), RenderColor, 0.f, FLinearColor(), FVector2D(), bCentreX, bCentreY, bOutlined, { 0.f, 0.f, 0.f, 1.f } };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawText2(FVector2D ScreenPosition, FLinearColor RenderColor, FString RenderText, FVector2D scale = { 1.f,1.f }, bool bCentered = true, bool bOutlined = true) {

		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawText");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}
		struct
		{
			uintptr_t RenderFont;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			FString                                     RenderText;                                               // (Parm, ZeroConstructor)
			FVector2D                                   ScreenPosition;                                           // (Parm, ZeroConstructor, IsPlainOldData)
			FVector2D                                   Scale;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
			FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Kerning;                                                  // (Parm, ZeroConstructor, IsPlainOldData)
			FLinearColor                                ShadowColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			FVector2D                                   ShadowOffset;                                             // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bCentreX;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bCentreY;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bOutlined;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			FLinearColor                                OutlineColor;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		UFont* RenderFont = MediumFont;
		params.RenderText = RenderText;
		params.ScreenPosition = { ScreenPosition.X, ScreenPosition.Y };
		params.Scale = scale;
		params.RenderColor = { (float)RenderColor.R, (float)RenderColor.G, (float)RenderColor.B, (float)RenderColor.A };
		params.Kerning = false;
		params.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
		params.ShadowOffset = { ScreenPosition.X + 1.5f, ScreenPosition.Y + 1.5f };

		if (bCentered)
		{
			params.bCentreX = true;
			params.bCentreY = true;
		}
		else
		{
			params.bCentreX = false;
			params.bCentreY = false;
		}

		params.bOutlined = bOutlined;
		params.OutlineColor = { 0.f, 0.f, 0.f, 1.f };

		this->ProcessEvent(Function, &params);
	}

	void K2_DrawPolygon(UTexture* RenderTexture, FVector2D ScreenPosition, FVector2D Radius, int32_t NumberOfSides, FLinearColor RenderColor)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawPolygon");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UTexture* RenderTexture;
			FVector2D ScreenPosition;
			FVector2D Radius;
			int32_t NumberOfSides;
			FLinearColor RenderColor;
		} Params = { RenderTexture, ScreenPosition, Radius, NumberOfSides, RenderColor };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawBox(FVector2D ScreenPosition, FVector2D ScreenSize, float Thickness, FLinearColor RenderColor) {
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawBox");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct K2_DrawBox_Struct {
			FVector2D ScreenPosition;
			FVector2D ScreenSize;
			float Thickness;
			FLinearColor RenderColor;
		}params;

		params.ScreenPosition = ScreenPosition;
		params.ScreenSize = ScreenSize;
		params.Thickness = Thickness;
		params.RenderColor = RenderColor;

		this->ProcessEvent(Function, &params);
	}

	void K2_DrawTexture(UTexture* RenderTexture, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize, FLinearColor RenderColor, EBlendMode BlendMode, float Rotation, FVector2D PivotPoint)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_DrawTexture");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct K2_DrawTexture_Params
		{
		public:
			UObject* RenderTexture;
			FVector2D ScreenPosition;
			FVector2D ScreenSize;
			FVector2D CoordinatePosition;
			FVector2D CoordinateSize;
			FLinearColor RenderColor;
			EBlendMode BlendMode;
			float Rotation;
			FVector2D PivotPoint;
		};

		K2_DrawTexture_Params Params;
		Params.RenderTexture = RenderTexture;
		Params.ScreenPosition = ScreenPosition;
		Params.ScreenSize = ScreenSize;
		Params.CoordinatePosition = CoordinatePosition;
		Params.CoordinateSize = CoordinateSize;
		Params.RenderColor = RenderColor;
		Params.BlendMode = BlendMode;
		Params.Rotation = Rotation;
		Params.PivotPoint = PivotPoint;

		this->ProcessEvent(Function, &Params);
	}

	FVector K2_Project(FVector WorldLocation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Canvas.K2_Project");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector WorldLocation;
			FVector ReturnValue;
		} Params = { WorldLocation };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void DrawCircle(FVector2D position, int radius, int numsides, FLinearColor color)
	{
		float Step = M_PI * 2.0 / numsides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < M_PI * 2.0; a += Step)
		{
			float X1 = radius * NoCRT::c_cos(a) + position.X;
			float Y1 = radius * NoCRT::c_sin(a) + position.Y;
			float X2 = radius * NoCRT::c_cos(a + Step) + position.X;
			float Y2 = radius * NoCRT::c_sin(a + Step) + position.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			this->K2_DrawLine(FVector2D({ V[Count].X, V[Count].Y }), FVector2D({ X2, Y2 }), 1.5f, color);
		}
	}

};



class UFortRuntimeOptions : public UObject
{
public:

};



class AHUD : public UObject
{
public:

};

class UActorComponent : public UObject
{
public:

};

class UCameraComponent : public UActorComponent
{
public:

};

class APlayerCameraManager : public UObject
{
public:

	float GetFOVAngle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerCameraManager.GetFOVAngle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetCameraLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerCameraManager.GetCameraLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator GetCameraRotation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerCameraManager.GetCameraRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AActor : public UObject
{
public:

	void K2_SetActorRelativeRotation(FRotator NewRelativeRotation, bool bTeleport = true)
	{
		//struct FRotator NewRelativeRotation, bool bSweep, struct FHitResult& SweepHitResult, bool bTeleport
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_SetActorRelativeRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}
		// Actor.K2_SetActorRelativeRotation
		struct {
			FRotator NewRelativeRotation;
			bool bSweep;
			FHitResult SweepHitResult;
			bool bTeleport;
		} Params = { NewRelativeRotation, false, FHitResult(), bTeleport };

		this->ProcessEvent(Function, &Params);
	}

	bool WasRecentlyRendered(float Tolerance)
	{
		struct
		{
			float Tolerance;
			bool ReturnValue;
		} Params = { Tolerance };

		static UObject* Function;
		Function = FindObjectSingle<UObject*>(Encrypt(L"Actor.WasRecentlyRendered"));
		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	FString GetPlayerNameSafe(AActor* AActor, UObject* playernameprivate)
	{
		struct {
			UObject* AActor;
			FString return_value;
		} params = { AActor };

		static UObject* function;
		if (!function) function = playernameprivate;
		this->ProcessEvent(function, &params);
		return params.return_value;
	}

	FString GetPlayerOrObjectNameSafe(AActor* AActor)
	{
		struct {
			UObject* AActor;
			FString ReturnValue;
		} params = { AActor };

		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.GetPlayerOrObjectNameSafe");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		this->ProcessEvent(Function, &params);
		return params.ReturnValue;
	}

	FString GetPlayerNameSafe2(AActor* AActor, UObject* FunctionPtr)
	{
		struct {
			UObject* AActor;
			FString return_value;
		} params = { AActor };

		static UObject* function;

		if (FunctionPtr && !function)
		{
			function = FunctionPtr;
		}

		if (function)
		{
			this->ProcessEvent(function, &params);
			return params.return_value;
		}

		return FString(L"error");
	}


	void DisableInput(UObject* PlayerController)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.DisableInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* PlayerController;
		} Params = { PlayerController };

		this->ProcessEvent(Function, &Params);
	}

	void EnableInput(UObject* PlayerController)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.EnableInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* PlayerController;
		} Params = { PlayerController };

		this->ProcessEvent(Function, &Params);
	}

	FVector GetVelocity()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.GetVelocity");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_TeleportTo(FVector DestLocation, FRotator DestRotation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_TeleportTo");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector DestLocation;
			FRotator DestRotation;
			bool ReturnValue;
		} Params = { DestLocation, DestRotation };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector K2_GetActorLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_GetActorLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_SetActorLocationAndRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector NewLocation;
			FRotator NewRotation;
			bool bSweep;
			FHitResult SweepHitResult;
			bool bTeleport;
			bool ReturnValue;
		} Params = { NewLocation, NewRotation, false, FHitResult(), true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_SetActorLocation(FVector new_location, bool sweep, uint8_t sweep_hit_result, bool teleport) {
		static UObject* function;
		if (!function)
			function = UObject::FindObjectSingle<UObject*>(L"Actor.K2_SetActorLocation");

		struct { FVector a1; bool a2; uint8_t a3; bool a4; bool ret; } params;
		params.a1 = new_location;
		params.a2 = sweep;
		params.a3 = sweep_hit_result;
		params.a4 = teleport;

		this->ProcessEvent(function, &params);
		return params.ret;
	}

	bool SetActorLocation(FVector new_location, bool sweep, uint8_t sweep_hit_result, bool teleport) {
		static UObject* function;
		if (!function)
			function = UObject::FindObjectSingle<UObject*>(L"Actor.K2_SetActorLocation");

		struct { FVector a1; bool a2; uint8_t a3; bool a4; bool ret; } params;
		params.a1 = new_location;
		params.a2 = sweep;
		params.a3 = sweep_hit_result;
		params.a4 = teleport;

		this->ProcessEvent(function, &params);
		return params.ret;
	}

	bool K2_SetActorRotation(FRotator NewRotation, bool bTeleportPhysics)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_SetActorRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator& NewRotation;
			bool bTeleportPhysics;
			bool ReturnValue;
		} Params = { NewRotation, bTeleportPhysics };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator K2_GetActorRotation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.K2_GetActorRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetActorTickEnabled(bool bEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.SetActorTickEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bEnabled;
		} Params = { bEnabled };

		this->ProcessEvent(Function, &Params);
	}

	void SetActorTickInterval(float TickInterval)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.SetActorTickInterval");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float TickInterval;
		} Params = { TickInterval };

		this->ProcessEvent(Function, &Params);
	}

	//struct FVector GetActorScale3D(); // Function Engine.Actor.GetActorScale3D // (Final|RequiredAPI|Native|Public|HasDefaults|BlueprintCallable|BlueprintPure|Const) // @ game+0x37a77d4

	FVector GetActorScale3D()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.GetActorScale3D");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params;

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetActorScale3D(FVector NewScale3D)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.SetActorScale3D");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector& NewScale3D;
		} Params = { NewScale3D };

		this->ProcessEvent(Function, &Params);
	}

	void SetNetDormancy(ENetDormancy NewDormancy)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Actor.SetNetDormancy");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			ENetDormancy NewDormancy;
		} Params = { NewDormancy };

		this->ProcessEvent(Function, &Params);
	}
};

// ScriptStruct Engine.MinimalViewInfo
// Size: 0x800 (Inherited: 0x00)
struct FMinimalViewInfo {
	FVector Location; // 0x00(0x18)
	FRotator Rotation; // 0x18(0x18)
	float FOV; // 0x30(0x04)
	float DesiredFOV; // 0x34(0x04)
	float OrthoWidth; // 0x38(0x04)
	bool bAutoCalculateOrthoPlanes; // 0x3c(0x01)
	char pad_3D[0x3]; // 0x3d(0x03)
	float AutoPlaneShift; // 0x40(0x04)
	bool bUpdateOrthoPlanes; // 0x44(0x01)
	bool bUseCameraHeightAsViewTarget; // 0x45(0x01)
	char pad_46[0x2]; // 0x46(0x02)
	float OrthoNearClipPlane; // 0x48(0x04)
	float OrthoFarClipPlane; // 0x4c(0x04)
	float PerspectiveNearClipPlane; // 0x50(0x04)
	float AspectRatio; // 0x54(0x04)
	char pad_58[0x8]; // 0x58(0x08)
	char bConstrainAspectRatio : 1; // 0x60(0x01)
	char bUseFieldOfViewForLOD : 1; // 0x60(0x01)
	char pad_60_2 : 6; // 0x60(0x01)
	char pad_61[0x3]; // 0x61(0x03)
	void* ProjectionMode; // 0x64(0x01) // enum class ECameraProjectionMode
	char pad_65[0x3]; // 0x65(0x03)
	float PostProcessBlendWeight; // 0x68(0x04)
	char pad_6C[0x4]; // 0x6c(0x04)
	void* PostProcessSettings; // 0x70(0x6f0) /// FPostProcessSettings
	FVector2D OffCenterProjectionOffset; // 0x760(0x10)
	char pad_770[0x90]; // 0x770(0x90)
};

class UFortPlayerStateComponent_Habanero : public AActor
{
public:
	FRankedProgressReplicatedData GetRankedProgress()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerStateComponent_Habanero.GetRankedProgress");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRankedProgressReplicatedData ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

// Enum FortniteGame.EFortCustomGender
enum class EFortCustomGender : uint8 {
	Invalid = 0,
	Male = 1,
	Female = 2,
	Both = 3,
	EFortCustomGender_MAX = 4
};

class AFortPlayerStateAthena : public AActor
{
public:

	int32_t GetKillScore()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerStateAthena.GetKillScore");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t ReturnValue;
		} Params;

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

// Class FortniteGame.FortClientOnlyActor
// Size: 0x298 (Inherited: 0x290)
struct AFortClientOnlyActor : AActor
{
	char pad_290[0x8]; // 0x290(0x08)
};

// Class Engine.CurveTable
// Size: 0xa0 (Inherited: 0x28)
struct UCurveTable : UObject {
	char pad_28[0x78]; // 0x28(0x78)
};

struct FCurveTableRowHandle {
	struct UCurveTable* CurveTable; // 0x00(0x08)
	struct FName RowName; // 0x08(0x04)
	char pad_C[0x4]; // 0x0c(0x04)
};

// Class FortniteGame.FortTracerBase
// Size: 0x370 (Inherited: 0x298)
struct AFortTracerBase : AFortClientOnlyActor
{
	struct UMovementComp_Tracer* TracerMovementComponent; // 0x298(0x08)
	struct UStaticMeshComponent* TracerMesh; // 0x2a0(0x08)
	struct UBulletWhipTrackerComponentBase* BulletWhipTrackerComponent; // 0x2a8(0x08)
	struct FCurveTableRowHandle Speed; // 0x2b0(0x10)
	float SpeedScaleMinRange; // 0x2c0(0x04)
	float SpeedScaleMaxRange; // 0x2c4(0x04)
	float SpeedScaleMinMultiplier; // 0x2c8(0x04)
	float SpeedScaleMaxMultiplier; // 0x2cc(0x04)
	struct FVector MeshScaleMult; // 0x2d0(0x18)
	float MeshScaleTime; // 0x2e8(0x04)
	char pad_2EC[0x4]; // 0x2ec(0x04)
	struct UBulletWhipTrackerComponentBase* BulletWhipTrackerComponentClass; // 0x2f0(0x08)
	char bScaleOnDeath : 1; // 0x2f8(0x01)
	char bScaleSpeed : 1; // 0x2f8(0x01)
	char bScaledUp : 1; // 0x2f8(0x01)
	char bDead : 1; // 0x2f8(0x01)
	char bOwnedByPool : 1; // 0x2f8(0x01)
	char pad_2F8_5 : 3; // 0x2f8(0x01)
	char pad_2F9[0x67]; // 0x2f9(0x67)
	float currentScale; // 0x360(0x04)
	char pad_364[0xc]; // 0x364(0x0c)

	//void Init(struct FVector& Start, struct FVector& End); // Function FortniteGame.FortTracerBase.Init // (Final|Native|Public|HasOutParms|HasDefaults|BlueprintCallable) // @ game+0x9f95220
	void Init(struct FVector& Start, struct FVector& End)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortTracerBase.Init");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector& Start;
			FVector& End;
		} Params = { Start, End };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);

			if (Params.Start)
			{
				Start = Params.Start;
			}

			if (Params.End)
			{
				End = Params.End;
			}
		}
	}

};

class AFortPlayerState : public AFortPlayerStateAthena
{
public:
	UFortPlayerStateComponent_Habanero* HabaneroComponent()
	{
		return *(UFortPlayerStateComponent_Habanero**)(this + Offsets::HabaneroComponent);
	}

	FString GetPlatform()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerState.GetPlatform");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);
		if (Params.ReturnValue)
		{
			return Params.ReturnValue;
		}

		return FString(L"BOT/AI");
	}

	FString GetRankString()
	{
		FString rankStr = L"Unranked";
		auto HC = this->HabaneroComponent();
		if (HC)
		{
			int32_t RankProgress = HC->GetRankedProgress().Rank;
			if (RankProgress >= 0 && RankProgress < 18)
			{
				static const FString ranks[] =
				{
					FString(L"Bronze 1"),
					FString(L"Bronze 2"),
					FString(L"Bronze 3"),
					FString(L"Silver 1"),
					FString(L"Silver 2"),
					FString(L"Silver 3"),
					FString(L"Gold 1"),
					FString(L"Gold 2"),
					FString(L"Gold 3"),
					FString(L"Platinum 1"),
					FString(L"Platinum 2"),
					FString(L"Platinum 3"),
					FString(L"Diamond 1"),
					FString(L"Diamond 2"),
					FString(L"Diamond 3"),
					FString(L"Elite"),
					FString(L"Champion"),
					FString(L"Unreal"),
					FString(L"Unranked")
				};

				rankStr = ranks[RankProgress];
			}
			else
			{
				rankStr = FString(L"Unranked");
			}
		}
		return rankStr;
	}
};

class AFortPlayerStateZone : public AFortPlayerState
{
public:
	int32_t GetSpectatorCount()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerStateZone.GetSpectatorCount");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void GetHealthAndShields(float& Health, float& HealthMax, float& Shield, float& ShieldMax)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerStateZone.GetHealthAndShields");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float Health;
			float HealthMax;
			float Shield;
			float ShieldMax;

		} Params = { };

		this->ProcessEvent(Function, &Params);

		Health = Params.Health;
		HealthMax = Params.HealthMax;
		Shield = Params.Shield;
		ShieldMax = Params.ShieldMax;

	}
};

class APlayerState : public AFortPlayerStateZone
{
public:

	FString GetPlayerName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerState.GetPlayerName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;




	}


	int GetTeamIndex()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerState.GetTeamIndex");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}


	bool IsABot()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerState.IsABot");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UMaterialInterface : public UObject
{
public:

};

class UMaterial : public UMaterialInterface
{
public:

};

class UMaterialInstance : public UMaterialInterface
{

};


class UMaterialInstanceDynamic : public UMaterialInstance
{
public:
	void SetVectorParameterValue(FName ParameterName, FLinearColor& Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"MaterialInstanceDynamic.SetVectorParameterValue");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName ParameterName;
			FLinearColor Value;
		} Params = { };

		Params.ParameterName = ParameterName;
		Params.Value = Value;

		this->ProcessEvent(Function, &Params);
	}

	FLinearColor K2_GetVectorParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"MaterialInstanceDynamic.K2_GetVectorParameterValue");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName ParameterName;
			FLinearColor ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetScalarParameterValue(FName ParameterName, float Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"MaterialInstanceDynamic.SetScalarParameterValue");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName ParameterName;
			float Value;
		} Params = { };

		Params.ParameterName = ParameterName;
		Params.Value = Value;

		this->ProcessEvent(Function, &Params);
	}

	float K2_GetScalarParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"MaterialInstanceDynamic.K2_GetScalarParameterValue");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName ParameterName;
			float ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UPrimitiveComponent : public UObject
{
public:
	void SetCustomDepthStencilValue(int Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PrimitiveComponent.SetCustomDepthStencilValue");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int Value;
		} Params = { Value };

		this->ProcessEvent(Function, &Params);
	}

	void SetRenderCustomDepth(bool bValue)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PrimitiveComponent.SetRenderCustomDepth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bValue;
		} Params = { bValue };

		this->ProcessEvent(Function, &Params);
	}

	UMaterialInstanceDynamic* CreateDynamicMaterialInstance(int32 ElementIndex, UMaterialInterface* Parent, FName OptionalName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PrimitiveComponent.CreateDynamicMaterialInstance");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32 ElementIndex;
			UMaterialInterface* Parent;
			FName OptionalName;
			UMaterialInstanceDynamic* ReturnValue;
		} Params = { ElementIndex, Parent, OptionalName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetMaterial(int32_t ElementIndex, UMaterialInterface* Material)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PrimitiveComponent.SetMaterial");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t ElementIndex;
			UMaterialInterface* Material;
		} Params = { ElementIndex, Material };

		this->ProcessEvent(Function, &Params);
	}

	UMaterialInterface* GetMaterial(int32_t ElementIndex)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PrimitiveComponent.GetMaterial");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t ElementIndex;
			UMaterialInterface* ReturnValue;
		} Params = { ElementIndex };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class USceneComponent : public UPrimitiveComponent
{
public:
	FVector GetSocketLocation(FName InSocketName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"SceneComponent.GetSocketLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName InSocketName;
			FVector ReturnValue;
		} Params{ InSocketName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetForwardVector()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"SceneComponent.GetForwardVector");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector K2_GetComponentLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"SceneComponent.K2_GetComponentLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UMeshComponent : public USceneComponent
{
public:
	TArray<UMaterialInterface*> GetMaterials()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"MeshComponent.GetMaterials");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			TArray<UMaterialInterface*> ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UStaticMeshComponent : public UMeshComponent
{
public:

};

class USkinnedMeshComponent : public UStaticMeshComponent
{
public:
	FName GetBoneName(int32 BoneIndex)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"SkinnedMeshComponent.GetBoneName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32 BoneIndex;
			FName ReturnValue;
		} Params{ BoneIndex };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetNumBones()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"SkinnedMeshComponent.GetNumBones");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class USkeletalMeshComponent : public USkinnedMeshComponent
{
public:

	FVector GetBoneLocation(int BoneID)
	{
		return this->GetSocketLocation(this->GetBoneName(BoneID));
	}
};

class UItemDefinitionBase : public UObject
{
public:
	FText DisplayName()
	{
		return *(FText*)(this + 0x40);
	}
};

class AFortLightweightProjectileConfig : public AActor
{
public:

};

class AFortLightweightProjectileManager : public UObject
{
public:

};



// Enum FortniteGame.EFortWeaponTriggerType
enum class EFortWeaponTriggerType : uint8_t
{
	OnPress = 0,
	Automatic = 1,
	OnRelease = 2,
	OnPressAndRelease = 3,
	Custom = 4,
	EFortWeaponTriggerType_MAX = 5
};

class UFortItemDefinition : public UItemDefinitionBase
{
public:

	// enum class EFortRarity GetRarity(); // Function FortniteGame.FortItemDefinition.GetRarity // (Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0x6a6bf60

	//EFortRarity GetRarity(); // Function FortniteGame.FortItemDefinition.GetRarity // (Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0x6954248

	EFortRarity GetRarity()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortItemDefinition.GetRarity");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			EFortRarity ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	TSoftObjectPtr<UTexture2D> GetSmallPreviewImage()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortItemDefinition.GetSmallPreviewImage");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			TSoftObjectPtr<UTexture2D> ReturnValue;
		} Params = { true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UFortWeaponRangedItemDefinition : public UFortItemDefinition
{
public:

	bool ShouldUsePerfectAimWhenTargetingMinSpread()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponRangedItemDefinition.ShouldUsePerfectAimWhenTargetingMinSpread");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;

		return 0.f;
	}

	bool HasLightweightProjectile()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponRangedItemDefinition.HasLightweightProjectile");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UFortWeaponItemDefinition : public UFortWeaponRangedItemDefinition
{
public:
	// enum class EFortWeaponTriggerType GetTriggerType(); // Function FortniteGame.FortWeaponItemDefinition.GetTriggerType // (Final|Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0x9837a70

	EFortWeaponTriggerType GetTriggerType()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponItemDefinition.GetTriggerType");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		if (Function)
		{
			struct
			{
				EFortWeaponTriggerType ReturnValue;
			} Params;

			this->ProcessEvent(Function, &Params);

			return Params.ReturnValue;
		}

		return EFortWeaponTriggerType::EFortWeaponTriggerType_MAX; // error
	}
};

class FFortBaseWeaponStats : public UObject
{
public:

};

// Enum FortniteGame.EFortWeaponOverheatState
enum class EFortWeaponOverheatState : uint8 {
	None = 0,
	Heating = 1,
	Cooling = 2,
	Overheated = 3,
	EFortWeaponOverheatState_MAX = 4
};


class AFortWeapon : public AActor
{
public:
	float GetProjectileGravity()
	{
		return *(float*)(this + Offsets::ProjectileGravity);
	}

	float GetProjectileSpeed()
	{
		return *(float*)(this + Offsets::ProjectileSpeed);
	}

	EFortWeaponCoreAnimation GetWeaponCoreAnimation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetCoreAnimation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			EFortWeaponCoreAnimation ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	UFortWeaponItemDefinition* GetWeaponData()
	{
		return *(UFortWeaponItemDefinition**)(this + Offsets::WeaponData);
	}

	void SetTraceThroughPawnsLimit(int32_t NewTraceThroughPawnsLimit)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetTraceThroughPawnsLimit");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t NewTraceThroughPawnsLimit;
		} Params = { NewTraceThroughPawnsLimit };

		this->ProcessEvent(Function, &Params);
	}

	void SetTraceThroughPawns(bool bNewTraceThroughPawns)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetTraceThroughPawns");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewTraceThroughPawns;
		} Params = { bNewTraceThroughPawns };

		this->ProcessEvent(Function, &Params);
	}

	void SetTraceThroughLandscapeLimit(int32_t NewTraceThroughTerrainLimit)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetTraceThroughLandscapeLimit");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t NewTraceThroughTerrainLimit;
		} Params = { NewTraceThroughTerrainLimit };

		this->ProcessEvent(Function, &Params);
	}

	void SetTraceThroughBuildingsLimit(int32_t NewTraceThroughBuildingsLimit)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetTraceThroughBuildingsLimit");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t NewTraceThroughBuildingsLimit;
		} Params = { NewTraceThroughBuildingsLimit };

		this->ProcessEvent(Function, &Params);
	}

	float GetTimeToNextFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetTimeToNextFire");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsProjectileWeapon()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.IsProjectileWeapon");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FColor GetReticleColor()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetReticleColor");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FColor ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsTargeting()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.IsTargeting");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetTargeting(bool bNewIsTargeting)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetTargeting");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);
	}


	FVector GetTargetingSourceLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetTargetingSourceLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void EnableAltCenterReticle(bool bNewEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponRanged.EnableAltCenterReticle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewEnabled;
		} Params = { bNewEnabled };

		this->ProcessEvent(Function, &Params);
	}

	void EnableAltOuterReticle(bool bNewEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponRanged.EnableAltOuterReticle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewEnabled;
		} Params = { bNewEnabled };

		this->ProcessEvent(Function, &Params);
	}

	void EnableCornersReticle(bool bNewEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeaponRanged.EnableCornersReticle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewEnabled;
		} Params = { bNewEnabled };

		this->ProcessEvent(Function, &Params);
	}

	bool CanFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.CanFire");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bPrimaryFire;
			bool ReturnValue;
		} Params = { true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetMuzzleLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetMuzzleLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int PatternIndex;
			FVector ReturnValue;
		} Params{ 0 };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetBulletsPerClip()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetBulletsPerClip");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//void SetIsReloadingWeapon(bool bReloading); // Function FortniteGame.FortWeapon.SetIsReloadingWeapon // (Final|Native|Public|BlueprintCallable) // @ game+0x9f58cd4

	void SetIsReloadingWeapon(bool bReloading = false)
	{
		//FortniteGame.FortWeapon.SetIsReloadingWeapon
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.SetIsReloadingWeapon");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bReloading;
		} Params = { };

		this->ProcessEvent(Function, &Params);
	}

	bool IsReloading()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.IsReloading");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetMagazineAmmoCount()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetMagazineAmmoCount");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetRemainingAmmo()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortWeapon.GetRemainingAmmo");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UFortVehicleFuelComponent : public AActor
{
public:

};

class UFortPhysicsVehicleConfigs : public AActor
{
public:
};

class AFortAthenaVehicle : public AActor
{
public:


	//struct FString GetDisplayName(struct UObject* Object); // Function FortniteGame.FortAthenaVehicle.GetDisplayName // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x2708490

	FString GetDisplayName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.GetDisplayName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	bool SetVehicleStencilHighlighted(AActor* PotentialVehicle, bool bHighlighted, int32_t StencilValueOverride)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.SetVehicleStencilHighlighted");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AActor* PotentialVehicle;
			bool bHighlighted;
			int32_t StencilValueOverride;
			bool ReturnValue;
		} Params = { PotentialVehicle, bHighlighted, StencilValueOverride };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetSteeringAngle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.GetSteeringAngle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool CanContainPlayers()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.CanContainPlayers");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetHealth()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.GetHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetMaxHealth()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAthenaVehicle.GetMaxHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class APawn : public AActor
{
public:
	USceneComponent* RootComponent()
	{
		return *(USceneComponent**)(this + Offsets::RootComponent);
	}

	APlayerState* PlayerState()
	{
		return *(APlayerState**)(this + Offsets::PlayerState);
	}

	void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Pawn.AddMovementInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector& WorldDirection;
			float ScaleValue;
			bool bForce;
		} Params = { WorldDirection, ScaleValue, bForce };

		this->ProcessEvent(Function, &Params);
	}
};

class ACharacter : public APawn
{
public:
	USkeletalMeshComponent* Mesh()
	{
		return *(USkeletalMeshComponent**)(this + Offsets::Mesh);
	}

	void Jump()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Character.Jump");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{

		} Params = { };

		this->ProcessEvent(Function, &Params);
	}

	bool CanCrouch()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Character.CanCrouch");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool CanJump()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Character.CanJump");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPawn : public ACharacter
{
public:

	AFortWeapon* CurrentWeapon()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.GetCurrentWeapon");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AFortWeapon* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void PrototypeCharacterMovement(EMovementMode MovementMode, FVector LaunchVelocity)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.PrototypeCharacterMovement");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			EMovementMode MovementMode;
			FVector& LaunchVelocity;
		} Params = { MovementMode, LaunchVelocity };

		this->ProcessEvent(Function, &Params);
	}

	float GetHealth()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.GetHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetShield()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.GetShield");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	TArray<AFortWeapon*> GetCurrentWeaponList()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.GetCurrentWeaponList");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			TArray<AFortWeapon*> ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsDead()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.IsDead");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsSlidingEnabled()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.IsSlidingEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsDBNO()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.IsDBNO");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	uint8_t GetTeam()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.GetTeam");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void PawnStartFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.PawnStartFire");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			char FireModeNum;
		} Params = { 1 };

		this->ProcessEvent(Function, &Params);
	}

	void PawnStopFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPawn.PawnStopFire");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			char FireModeNum;
		} Params = { 1 };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortAIPawn : public AFortPawn
{
public:
	void ResetAIRotationRateToDefault()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAIPawn.ResetAIRotationRateToDefault");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{

		} Params = { };

		this->ProcessEvent(Function, &Params);
	}

	void SetCanInteract(bool CanInteract)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAIPawn.SetCanInteract");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool CanInteract;
		} Params = { CanInteract };

		this->ProcessEvent(Function, &Params);
	}

	void SetCanSleep(bool InCanSleep)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortAIPawn.SetCanSleep");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool InCanSleep;
		} Params = { InCanSleep };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortPlayerPawn : public AFortPawn
{
public:

	void OnTacticalSprintStarted()
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.OnTacticalSprintStarted");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{

		} Params = { };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	bool IsPassengerInVehicle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsPassengerInVehicle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsParachuteOpen()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsParachuteOpen");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsParachuteOpenAndVisible()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsParachuteOpenAndVisible");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//float GetMaxStamina(); // Function FortniteGame.FortPlayerPawn.GetMaxStamina // (Final|RequiredAPI|Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0xa025dd0

	float GetMaxStamina()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.GetMaxStamina");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	// 	float GetStamina(); // Function FortniteGame.FortPlayerPawn.GetStamina // (Final|RequiredAPI|Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0xa025fb4


	float GetStamina()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.GetStamina");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsInVehicle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsInVehicle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//struct AActor* GetVehicleActor(); // Function FortniteGame.FortPlayerPawn.GetVehicleActor // (Final|RequiredAPI|Native|Public|BlueprintCallable|BlueprintPure|Const) // @ game+0x34068f4

	AActor* GetVehicleActor()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.GetVehicleActor");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AActor* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//bool GetHeroDisplayNameString(struct FString& HeroDisplayName); // Function FortniteGame.FortPlayerPawn.GetHeroDisplayNameString // (Final|Native|Public|HasOutParms|BlueprintCallable|BlueprintPure|Const) // @ game+0xa0254b4

	bool GetHeroDisplayNameStringSDK(FString& HeroDisplayName)
	{

		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.GetHeroDisplayNameString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString ReturnValue;
		} Params = { };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);

			HeroDisplayName = Params.ReturnValue;
			return true;
		}

		return false;
	}

	bool GetHeroDisplayNameString(FString& HeroName)
	{

		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.GetHeroDisplayNameString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString HeroName;
			bool ReturnValue;
		} Params;

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
			if (Params.ReturnValue)
			{
				HeroName = Params.HeroName;
				return Params.ReturnValue;
			}
		}

		return (FString)L"Error";
	}

	bool IsDrivingVehicle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsDrivingVehicle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//void SetCrouch(bool bNewValue, bool bUseActionOnHoldInput); // Function FortniteGame.FortPlayerPawn.SetCrouch // (Final|Native|Public|BlueprintCallable) // @ game+0xa02c6f8

	// im pretty sure bUseActionOnHoldInput = use input for action thing when held, aka slide.
	void SetCrouch(bool bNewValue, bool bUseActionOnHoldInput = false)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.SetCrouch");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewValue;
			bool bUseActionOnHoldInput;
		} Params = { bNewValue, bUseActionOnHoldInput };

		this->ProcessEvent(Function, &Params);
	}

	bool IsSprinting()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsSprinting");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}


	bool IsSkyDiving()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsSkyDiving");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsSkydivingFromBus()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerPawn.IsSkydivingFromBus");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerPawnAthena : public AFortPlayerPawn
{
public:

};

class APlayerPawn_Athena_Generic_Parent_C : public AFortPlayerPawnAthena
{
public:
};

class APlayerPawn_Athena_Generic_C : public APlayerPawn_Athena_Generic_Parent_C
{
public:
};

class APlayerPawn_Athena_C : public APlayerPawn_Athena_Generic_C
{
public:


};

class ASpectatorPawn : public APlayerPawn_Athena_C
{
public:

};

class BGA_Athena_SCMachine_C : public AActor
{
public:

};

class AFortPickupEffect : public AActor
{
public:

};

class AFortPlayerMarkerBase : public AActor
{
public:

};


class AFortPickup : public AActor
{
public:
	UFortItemDefinition* PrimaryPickupItemEntry()
	{
		return *(UFortItemDefinition**)(this + 0x368 + 0x18);
	}
};

class UBaseBuildingStaticMeshComponent : public UMeshComponent
{
public:

};

class ABuildingSMActor : public AActor
{
public:
};

struct BuildingContainer_Bitfield
{
	char bForceSpawnLootOnDestruction : 1;
	char bForceTossLootOnSpawn : 1;
	char bAlreadySearched : 1;
	char bGivePickupsDirectlyToPlayer : 1;
	char bDisableSpawnLoot : 1;
	char bDoNotDropLootOnDestruction : 1;
	char bSkipRollForDestruction : 1;
};

class ABuildingContainer : public ABuildingSMActor
{
public:

};

struct BuildingWeakSpot_Bitfield
{
	char bHit : 1;
	char bFadeOut : 1;
	char bActive : 1;
};

class ABuildingWeakSpot : public AActor
{
public:
};

class AFortAthenaSupplyDrop : public AActor
{
public:

};

class ABuildingTrap : public AActor
{
public:
	float GetArmTime()
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingTrap.GetArmTime");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AAthenaSuperDingo : public AActor
{
public:

};

class ABuildingActor : public AActor
{
public:

	bool IsPlayerBuilt()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.IsPlayerBuilt");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params;

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool HasHealthLeft()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.HasHealthLeft");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params;

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetQuestHighlight(bool Enabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.SetQuestHighlight");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool Enabled;
		} Params = { Enabled };

		this->ProcessEvent(Function, &Params);
	}

	void SetSuppressHealthBar(bool bNewValue)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.SetSuppressHealthBar");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewValue;
		} Params = { bNewValue };

		this->ProcessEvent(Function, &Params);
	}

	float GetHealth()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.GetHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetMaxHealth()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.GetMaxHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void ForceBuildingHealth(float NewHealth)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.ForceBuildingHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float NewHealth;
		} Params = { NewHealth };

		this->ProcessEvent(Function, &Params);
	}

	void SetHealth(float NewHealth)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BuildingActor.SetHealth");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float NewHealth;
		} Params = { NewHealth };

		this->ProcessEvent(Function, &Params);
	}
};

class AController : public AActor
{
public:

	void SetIgnoreLookInput(bool bNewLookInput)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"Controller.SetIgnoreLookInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewLookInput;
		} Params = { bNewLookInput };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortPlayerController : public AController
{
public:

	UFortRuntimeOptions* GetRuntimeOptions()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortRuntimeOptions.GetRuntimeOptions");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UFortRuntimeOptions* ReturnValue;
		} Params = { };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
			return Params.ReturnValue;
		}
	}

	void ServerEmote(FName AssetName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerController.ServerEmote");

			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName AssetName;
		} params = { AssetName };

		this->ProcessEvent(Function, &params);
	}

	FVector GetLocationUnderReticle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortPlayerController.GetLocationUnderReticle");

			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector ReturnValue;
		} params = { };

		this->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}
};
class APlayerController : public AFortPlayerController
{
private:

	FText PlayerController_Conv_StringToText(FString InString)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetTextLibrary.Conv_StringToText");

			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InString;
			FText ReturnValue;
		} Params = { InString };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

public:
	void AddYawInput(float Val) {
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"APlayerControllerController.AddYawInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float Val;
		} Params = { Val };

		this->ProcessEvent(Function, &Params);
	}
	void AddRollInput(float Val) {
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"APlayerControllerController.AddRollInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float Val;
		} Params = { Val };

		this->ProcessEvent(Function, &Params);
	}

	struct PlayerController_ProjectWorldLocationToScreen final
	{
	public:
		struct FVector                                WorldLocation;                                     // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              ScreenLocation;                                    // 0x000C(0x0008)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bPlayerViewportRelative;                           // 0x0014(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          ReturnValue;                                       // 0x0015(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_16[0x2];                                       // 0x0016(0x0002)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	bool ProjectWorldLocationToScreen(const struct FVector& WorldLocation, struct FVector2D* ScreenLocation, bool bPlayerViewportRelative) const
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.ProjectWorldLocationToScreen");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt());
			EncryptedFunction.clear();
		}

		PlayerController_ProjectWorldLocationToScreen Parms{};
		Parms.WorldLocation = std::move(WorldLocation);
		Parms.bPlayerViewportRelative = bPlayerViewportRelative;

		const_cast<APlayerController*>(this)->ProcessEvent(Function, &Parms);

		if (ScreenLocation != nullptr)
			*ScreenLocation = std::move(Parms.ScreenLocation);

		return Parms.ReturnValue;
	};

	void AddPitchInput(float Val) {
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"APlayerControllerController.AddPitchInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float Val;
		} Params = { Val };

		this->ProcessEvent(Function, &Params);
	}

	void SetAimRotation(FRotator NewRotation)
	{
		NewRotation.Normalize();

		*(FRotator*)(this + 0x528) = NewRotation;
		*(FRotator*)(this + 0x640) = NewRotation;
	}

	void ClientKickWithText(FString Reason)
	{
		if (this)
		{
			FText TextReason = this->PlayerController_Conv_StringToText(Reason);

			this->ClientReturnToMainMenuWithTextReason(TextReason);
		}
	}

	void ClientReturnToMainMenuWithTextReason(FText ReturnReason)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"APlayerControllerController.ClientReturnToMainMenuWithTextReason");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();

			if (!Function)
			{
				auto EncryptedFunction2 = Encrypt(L"PlayerController.ClientReturnToMainMenuWithTextReason");
				Function = FindObjectSingle<UObject*>(EncryptedFunction2.decrypt()); EncryptedFunction2.clear();
				if (!Function)
				{
					return;
				}
			}

		}

		struct
		{
			FText ReturnReason;
		} params = { ReturnReason };

		this->ProcessEvent(Function, &params);
	}

	APlayerPawn_Athena_C* AcknowledgedPawn()
	{
		return *(APlayerPawn_Athena_C**)(this + Offsets::AcknowledgedPawn);
	}

	APlayerCameraManager* PlayerCameraManager()
	{
		return *(APlayerCameraManager**)(this + Offsets::PlayerCameraManager);
	}

	ASpectatorPawn* GetSpectatorPawn()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.GetSpectatorPawn");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			ASpectatorPawn* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool w2s(FVector world_location, FVector2D* screen_location) {
		struct {
			FVector world_location;
			FVector2D screen_location;
			bool return_value;
		} params = { world_location, FVector2D(), true };

		static UObject* function;
		if (!function) function = FindObjectSingle<UObject*>(L"PlayerController.ProjectWorldLocationToScreen");
		this->ProcessEvent(function, &params);
		if (params.screen_location) {
			*screen_location = params.screen_location;
		}
		return params.return_value;
	}

	AHUD* GetHUD()
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.GetHUD");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AHUD* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void FOV(float NewFOV)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.FOV");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float NewFOV;
		} Params = { NewFOV };

		this->ProcessEvent(Function, &Params);
	}

	FVector2D GetMousePosition()
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.GetMousePosition");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float LocationX;
			float LocationY;
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return FVector2D(double(Params.LocationX), double(Params.LocationY));
	}

	bool IsInputKeyDown(FKey key)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.IsInputKeyDown");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FKey key;
			bool ReturnValue;
		} Params = { key };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void ClientIgnoreMoveInput(bool bIgnore)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.ClientIgnoreMoveInput");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bIgnore;
		} Params = { bIgnore };

		this->ProcessEvent(Function, &Params);
	}

	bool WasInputKeyJustPressed(FKey key)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.WasInputKeyJustPressed");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FKey key;
			bool ReturnValue;
		} Params = { key };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetMouseLocation(int32_t X, int32_t Y)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"PlayerController.SetMouseLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t X;
			int32_t Y;
		} Params = { X, Y };

		this->ProcessEvent(Function, &Params);
	}
};

class UGameViewportClient : public UObject
{
public:
	bool LineTraceSingle(UObject* World, const FVector& Start, const FVector& End)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.LineTraceSingle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;                                       // (ConstParm, Parm, ZeroConstructor)
			struct FVector                                     Start;                                                    // (ConstParm, Parm, ZeroConstructor, IsPlainOldData)
			struct FVector                                     End;                                                      // (ConstParm, Parm, ZeroConstructor, IsPlainOldData)
			TEnumAsByte<ETraceTypeQuery>                       TraceChannel;                                             // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bTraceComplex;                                            // (Parm, ZeroConstructor, IsPlainOldData)
			TArray<uint64_t>                                   ActorsToIgnore;                                           // (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm)
			TEnumAsByte<EDrawDebugTrace>                       DrawDebugType;                                            // (Parm, ZeroConstructor, IsPlainOldData)
			struct FHitResult                                  OutHit;                                                   // (Parm, OutParm, IsPlainOldData)
			bool                                               bIgnoreSelf;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			struct FLinearColor                                TraceColor;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			struct FLinearColor                                TraceHitColor;                                            // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              DrawTime;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			unsigned char                                      Padding[0x8];                                       // 0x00AE(0x0002) MISSED OFFSET
			bool                                               ReturnValue;
		} params;
		params.WorldContextObject = World;
		params.Start = Start;
		params.End = End;
		params.TraceChannel = ETraceTypeQuery::TraceTypeQuery2;
		params.bTraceComplex = true;
		params.ActorsToIgnore = TArray<uint64_t>();
		params.DrawDebugType = EDrawDebugTrace::None;
		params.bIgnoreSelf = true;
		params.TraceColor = FLinearColor();
		params.TraceHitColor = FLinearColor();
		params.DrawTime = 0.0f;

		this->ProcessEvent(Function, &params);

		return !params.ReturnValue;
	}

	UObject* World()
	{
		return *(UObject**)(this + Offsets::World);
	}
};

class UPlayer : public UObject
{
public:
	APlayerController* PlayerController()
	{
		return *(APlayerController**)(this + Offsets::PlayerController);
	}
};

class UFortClientSettingsRecord : public UObject
{
public:

	void SetAimAssistStrength(uint32_t NewValue)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetAimAssistStrength");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			uint32_t NewValue;
		} Params = { NewValue };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	void SetUseLegacyControls(bool NewUseLegacyControls)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetUseLegacyControls");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool NewUseLegacyControls;
		} Params = { NewUseLegacyControls };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	//void SetSmartBuildEnabled(bool bNewSmartBuildEnabled); // Function FortniteGame.FortClientSettingsRecord.SetSmartBuildEnabled // (Final|Native|Public) // @ game+0x93b5afc

	void SetSmartBuildEnabled(bool bNewSmartBuildEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetSmartBuildEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewSmartBuildEnabled;
		} Params = { bNewSmartBuildEnabled };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	//bool GetContextTutorialEnabled(); // Function FortniteGame.FortClientSettingsRecord.GetContextTutorialEnabled // (Final|Native|Public|Const) // @ game+0x9797c40

	//void SetContextTutorialEnabled(bool bEnabled); // Function FortniteGame.FortClientSettingsRecord.SetContextTutorialEnabled // (Final|Native|Public) // @ game+0x979c440

	bool GetContextTutorialEnabled()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetContextTutorialEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//bool GetUsePowerSavingMode(); // Function FortniteGame.FortClientSettingsRecord.GetUsePowerSavingMode // (Final|Native|Public|Const) // @ game+0x97998d0

	void SetMasterVolume(float InVolume) // // Function FortniteGame.FortClientSettingsRecord.SetMasterVolume // (Final|RequiredAPI|Native|Public) // @ game+0x97a4bb8
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetMasterVolume");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float InVolume;
		} Params = { InVolume };

		this->ProcessEvent(Function, &Params);
	}

	void SetMouseSensitivityY(float InSensitivityY) // // Function FortniteGame.FortClientSettingsRecord.SetMasterVolume // (Final|RequiredAPI|Native|Public) // @ game+0x97a4bb8
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetMouseSensitivityY");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float InSensitivityY;
		} Params = { InSensitivityY };

		this->ProcessEvent(Function, &Params);
	}

	void SetMouseSensitivityX(float InSensitivityX) // // Function FortniteGame.FortClientSettingsRecord.SetMasterVolume // (Final|RequiredAPI|Native|Public) // @ game+0x97a4bb8
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetMouseSensitivityX");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float InSensitivityX;
		} Params = { InSensitivityX };

		this->ProcessEvent(Function, &Params);

	}

	//float GetMouseSensitivityY(); // Function FortniteGame.FortClientSettingsRecord.GetMouseSensitivityY // (Final|Native|Public|Const) // @ game+0x9798fac
	//float GetMouseSensitivityX(); // Function FortniteGame.FortClientSettingsRecord.GetMouseSensitivityX // (Final|Native|Public|Const) // @ game+0x9798f94
	//float GetMasterVolume(); // Function FortniteGame.FortClientSettingsRecord.GetMasterVolume // (Final|RequiredAPI|Native|Public|Const) // @ game+0x9798ed4
	float GetMouseSensitivityX()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetMouseSensitivityX");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}


	void SetUsePowerSavingMode(bool bNewUsePowerSavingMode)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetUsePowerSavingMode");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewUsePowerSavingMode;
		} Params = { bNewUsePowerSavingMode };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}
	// Function FortniteGame.FortClientSettingsRecord.SetUsePowerSavingMode // (Final|Native|Public) // @ game+0x97a9b70

	//bool GetContextTutorialEnabled(); // Function FortniteGame.FortClientSettingsRecord.GetContextTutorialEnabled // (Final|Native|Public|Const) // @ game+0x9797c40

	//void SetContextTutorialEnabled(bool bEnabled); // Function FortniteGame.FortClientSettingsRecord.SetContextTutorialEnabled // (Final|Native|Public) // @ game+0x979c440


	float GetMouseSensitivityY()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetMouseSensitivityY");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//float GetMasterVolume(); // Function FortniteGame.FortClientSettingsRecord.GetMasterVolume // (Final|RequiredAPI|Native|Public|Const) // @ game+0x9798ed4

	float GetMasterVolume()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetMasterVolume");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}


	bool GetUsePowerSavingMode()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetUsePowerSavingMode");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}


	void SetContextTutorialEnabled(bool bEnabled)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetContextTutorialEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bEnabled;
		} Params = { bEnabled };

		this->ProcessEvent(Function, &Params);

	}

	//void SetPeripheralLightingEnabled(bool bEnable); // Function FortniteGame.FortClientSettingsRecord.SetPeripheralLightingEnabled // (Final|RequiredAPI|Native|Public) // @ game+0x97a577c

	void SetPeripheralLightingEnabled(bool bEnable)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetPeripheralLightingEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bEnable;
		} Params = { bEnable };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	bool GetPeripheralLightingEnabled()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.GetPeripheralLightingEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//bool GetPeripheralLightingEnabled(); // Function FortniteGame.FortClientSettingsRecord.GetPeripheralLightingEnabled // (Final|RequiredAPI|Native|Public|Const) // @ game+0x979900c



	void SetShowSessionIDWatermark(bool bInSessionIDWatermark)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetShowSessionIDWatermark");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bInSessionIDWatermark;
		} Params = { bInSessionIDWatermark };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	void SetAimAssistEnabled(bool bNewValue)
	{
		//KismetSystemLibrary.LogString
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortClientSettingsRecord.SetAimAssistEnabled");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			bool bNewValue;
		} Params = { bNewValue };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}
};

class UFortLocalPlayer : public UPlayer
{
public:
};

class USceneViewState : public UObject
{
public:
	FMatrix Projection()
	{
		return *(FMatrix*)(this + Offsets::Projection);
	}
};

class ULocalPlayer : public UFortLocalPlayer
{
public:
	UGameViewportClient* ViewportClient()
	{
		return *(UGameViewportClient**)(this + Offsets::ViewportClient);
	}

	USceneViewState* GetViewState()
	{
		TArray <USceneViewState*> ViewState = *(TArray <USceneViewState*>*)(this + 0xd0);

		return ViewState[1];
	}
};

class UGameInstance : public UObject
{
public:
	TArray <ULocalPlayer*> LocalPlayers()
	{
		return *(TArray <ULocalPlayer*>*)(this + Offsets::LocalPlayers);
	}
};

class UGameUserSettings : public UObject
{
public:

};

class UFortGameUserSettings : public UGameUserSettings
{
public:

};

class UFortKismetLibrary : public UObject
{
public:
	FString GetHumanReadableName(AActor* Actor)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.GetHumanReadableName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AActor* Actor;
			FString ReturnValue;
		} Params = { Actor };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//struct AFortTracerBase* SpawnTracerFromPool(struct UObject* WorldContextObject, struct AFortTracerBase* TracerClass, struct FVector& Origin, struct FVector& Destination, struct AFortPawn* Instigator); // Function FortniteGame.FortKismetLibrary.SpawnTracerFromPool // (Final|Native|Static|Public|HasOutParms|HasDefaults|BlueprintCallable) // @ game+0x9490b04
	AFortTracerBase* SpawnTracerFromPool(UObject* WorldContextObject, AFortTracerBase* TracerClass, FVector& Origin, FVector& Destination, AFortPawn* Instigator)
	{
		// Function FortniteGame.FortKismetLibrary.SpawnTracerFromPool
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.GetHumanReadableName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			AFortTracerBase* TracerClass;
			FVector& Origin;
			FVector& Destination;
			AFortPawn* Instigator;
			AFortTracerBase* ReturnValue;
		} Params = { WorldContextObject, TracerClass, Origin, Destination, Instigator };

		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	FHitResult GetPlayerAimPointHit(AFortPlayerController* SourcePlayer, float MaxRange, TArray<AActor*>& IgnoredActors)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.CheckLineOfSightToActorWithChannel");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AFortPlayerController* SourcePlayer;
			float MaxRange;
			TArray<AActor*>& IgnoredActors;
			FHitResult ReturnValue;
		} Params = { SourcePlayer, MaxRange, IgnoredActors };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool OnSameTeam(AActor* ActorA, AActor* ActorB)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.OnSameTeam");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			AActor* ActorA;
			AActor* ActorB;
			bool ReturnValue;
		} Params = { ActorA, ActorB };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool CheckLineOfSightToActorWithChannel(FVector SourcePos, AActor* Target, ECollisionChannel TargetFilterChannel, AActor* Source)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.CheckLineOfSightToActorWithChannel");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector SourcePos;
			AActor* Target;
			ECollisionChannel TargetFilterChannel;
			AActor* Source;
			bool ReturnValue;
		} Params = { SourcePos, Target, TargetFilterChannel, Source };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool GetSafeZoneLocation(UObject* WorldContextObject, int32_t SafeZoneIndex, FVector& OutLocation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.GetSafeZoneLocation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			int32_t SafeZoneIndex;
			FVector& OutLocation;
			bool ReturnValue;
		} Params = { WorldContextObject, SafeZoneIndex, OutLocation };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32_t GetCurrentSafeZonePhase(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"FortKismetLibrary.GetCurrentSafeZonePhase");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			int32_t ReturnValue;
		} Params = { WorldContextObject };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

};

class UKismetMathLibrary : public UObject
{
public:
	FVector VLerp(FVector A, FVector B, float Alpha)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.VLerp");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector A; FVector B; float Alpha;
			FVector ReturnValue;
		} Params = { A, B, Alpha };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FLinearColor LinearColorLerpUsingHSV(FLinearColor A, FLinearColor B, float Alpha)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.LinearColorLerpUsingHSV");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
		public:
			FLinearColor A;
			FLinearColor B;
			float Alpha;
			FLinearColor ReturnValue;
		}Parameters;

		Parameters.A = A;
		Parameters.B = B;
		Parameters.Alpha = Alpha;

		this->ProcessEvent(Function, &Parameters);

		return Parameters.ReturnValue;
	}

	FVector Vector_ClampSize2D(FVector& A, float Min, float Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Vector_ClampSize2D");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector& A;
			float Min;
			float Max;
		} Params = { A, Min, Max };

		this->ProcessEvent(Function, &Params);

		return Params.A;
	}

	FVector Conv_RotatorToVector(FRotator InRot)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Conv_RotatorToVector");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float NormalizeAxis(float Angle)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.NormalizeAxis");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float Angle;
			float ReturnValue;
		} Params = { Angle };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32_t FMod(double Dividend, double Divisor, double& Remainder)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.FMod");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double Dividend; double Divisor; double& Remainder;
			int32_t ReturnValue;
		} Params = { Dividend, Divisor, Remainder };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double FMin(double A, double B)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.FMin");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A, B;
			double ReturnValue;
		} Params = { A, B };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32_t Max(int32_t A, int32_t B)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Max");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32_t A, B;
			int32_t ReturnValue;
		} Params = { A, B };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double FMax(double A, double B)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.FMax");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A, B;
			double ReturnValue;
		} Params = { A, B };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator RLerp(FRotator A, FRotator B, float Alpha, bool bShortestPath)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Lerp");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator A; FRotator B; float Alpha; bool bShortestPath;
			FRotator ReturnValue;
		} Params = { A, B, Alpha, bShortestPath };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Lerp(double A, double B, double Alpha)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Lerp");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A, B, Alpha;
			double ReturnValue;
		} Params = { A, B, Alpha };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Clamp(double Value, double Min, double Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.FClamp");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double Value, Min, Max;
			double ReturnValue;
		} Params = { Value, Min, Max };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double sin(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.sin");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double cos(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.cos");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double acos(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.acos");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double tan(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.tan");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double asin(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.asin");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double atan(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.atan");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Atan2(double Y, double X)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Atan2");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double Y;
			double X;
			double ReturnValue;
		} Params = { Y, X };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double abs(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.abs");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Pow(double Base, double exp)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.MultiplyMultiply_FloatFloat");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double Base;
			double exp;
			double ReturnValue;
		} Params = { Base, exp };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Vector_Distance(FVector V1, FVector V2)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Vector_Distance");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { V1, V2 };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double Vector_Distance2D(FVector2D v1, FVector2D v2)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.Vector_Distance2D");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { FVector(v1.X, v1.Y, 0.0), FVector(v2.X, v2.Y, 0.0) };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double sqrt(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.sqrt");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double A;
			double ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32 RandomIntegerInRange(int32 Min, int32 Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.RandomIntegerInRange");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32 Min;
			int32 Max;
			int32 ReturnValue;
		} Params = { Min, Max };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double RandomFloatInRange(double Min, double Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.RandomFloatInRange");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			double Min;
			double Max;
			double ReturnValue;
		} Params = { Min, Max };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float DegreesToRadians(float A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.DegreesToRadians");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float A;
			float ReturnValue;
		} Params = { A };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetRightVector(FRotator InRot)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.GetRightVector");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetForwardVector(FRotator InRot)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.GetForwardVector");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator FindLookAtRotation(FVector& Start, FVector& Target)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.FindLookAtRotation");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector Start;
			FVector Target;
			FRotator ReturnValue;
		} Params = { Start, Target };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}



	FRotator RInterpTo_Constant(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.RInterpTo_Constant");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator RInterpTo(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.RInterpTo");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector VInterpTo(FVector& Current, FVector& Target, float DeltaTime, float InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetMathLibrary.VInterpTo");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FVector Current;
			FVector Target;
			float DeltaTime;
			float InterpSpeed;
			FVector ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UGameplayStatics : public UObject
{
public:

	UGameInstance* GetGameInstance(UObject* WorldContextObject)
	{

		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"GameplayStatics.GetGameInstance");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			UGameInstance* ReturnValue;
		} Params = { WorldContextObject };

		this->ProcessEvent(Function, &Params);

		//WindowsAPI::MessageBoxA(0, WindowsAPI::LPCSTR(std::to_string((uintptr_t)(void*)Params.ReturnValue).c_str()), "GameplayStatics.GetGameInstance", 0);

		return Params.ReturnValue;
	}

	TArray<UObject*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"GameplayStatics.GetAllActorsOfClass");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			UObject* ActorClass;
			TArray<UObject*> OutActors;
		} Params = { WorldContextObject, ActorClass };

		this->ProcessEvent(Function, &Params);

		return Params.OutActors;
	}

	bool ObjectIsA(UObject* Object, UObject* ObjectClass)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"GameplayStatics.ObjectIsA");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* Object;
			UObject* ObjectClass;
			bool ReturnValue;
		} Params = { Object, ObjectClass };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double GetWorldDeltaSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"GameplayStatics.GetWorldDeltaSeconds");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	double GetTimeSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"GameplayStatics.GetTimeSeconds");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetSystemLibrary : public UObject
{
public:

	//void PrintText(struct UObject* WorldContextObject, struct FText InText, bool bPrintToScreen, bool bPrintToLog, struct FLinearColor TextColor, float duration, struct FName Key); // Function Engine.KismetSystemLibrary.PrintText // (Final|RequiredAPI|Native|Static|Public|HasDefaults|BlueprintCallable) // @ game+0x6b5ca68

	FString GetUniqueDeviceId()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetUniqueDeviceId");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	//struct FString GetDisplayName(struct UObject* Object); // Function Engine.KismetSystemLibrary.GetDisplayName // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x2708490

	FString GetDisplayName(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetDisplayName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	//struct FString GetPlatformUserDir(); // Function Engine.KismetSystemLibrary.GetPlatformUserDir // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b2bbbc

	FString GetPlatformUserDir()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetPlatformUserDir");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	// 	bool FileExists(struct FString InPath); // Function Engine.BlueprintPathsLibrary.FileExists // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b2019c
	// bool DirectoryExists(struct FString InPath); // Function Engine.BlueprintPathsLibrary.DirectoryExists // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b15270

	// 	void LogString(struct FString InString, bool bPrintToLog); // Function Engine.KismetSystemLibrary.LogString // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable) // @ game+0x6b45864

	void LogString(FString InString, bool bPrintToLog = true)
	{
		//KismetSystemLibrary.LogString
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.LogString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InString;
			bool bPrintToLog;
		} Params = { InString, bPrintToLog };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
		}
	}

	bool FileExists(FString InPath)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BlueprintPathsLibrary.FileExists");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InPath;
			bool ReturnValue;
		} Params = { InPath };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
			return Params.ReturnValue;
		}

		return false;
	}

	bool DirectoryExists(FString InPath)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"BlueprintPathsLibrary.DirectoryExists");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InPath;
			bool ReturnValue;
		} Params = { InPath };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);
			return Params.ReturnValue;
		}

		return false;
	}

	FString GetPlatformUserName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetPlatformUserName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	FString GetDeviceId()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetDeviceId");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };


		this->ProcessEvent(Function, &Params);
		return Params.ReturnValue;
	}

	double GetGameTimeInSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetGameTimeInSeconds");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void GetComponentBounds(USceneComponent* Component, FVector* Origin, FVector* BoxExtent, float* SphereRadius)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetComponentBounds");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
		public:
			USceneComponent* Component;
			FVector Origin;
			FVector BoxExtent;
			float SphereRadius;
		}Parameters;

		Parameters.Component = Component;

		this->ProcessEvent(Function, &Parameters);

		if (Origin != nullptr)
			*Origin = Parameters.Origin;

		if (BoxExtent != nullptr)
			*BoxExtent = Parameters.BoxExtent;

		if (SphereRadius != nullptr)
			*SphereRadius = Parameters.SphereRadius;
	}

	bool LineTraceSingle(UObject* WorldContextObject, FVector Start, FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, TArray<AActor*> ActorsToIgnore, EDrawDebugTrace DrawDebugType, FHitResult* OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.LineTraceSingle");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
		public:
			UObject* WorldContextObject;
			FVector Start;
			FVector End;
			ETraceTypeQuery TraceChannel;
			bool bTraceComplex;
			uint8 Pad_1C02[0x6];
			TArray<AActor*> ActorsToIgnore;
			EDrawDebugTrace DrawDebugType;
			uint8 Pad_1C04[0x3];
			FHitResult OutHit;
			bool bIgnoreSelf;
			uint8 Pad_1C05[0x3];
			FLinearColor TraceColor;
			FLinearColor TraceHitColor;
			float DrawTime;
			bool ReturnValue;
			uint8 Pad_1C06[0x7];
		}Parms;

		Parms.WorldContextObject = WorldContextObject;
		Parms.Start = Start;
		Parms.End = End;
		Parms.TraceChannel = TraceChannel;
		Parms.bTraceComplex = bTraceComplex;
		Parms.ActorsToIgnore = ActorsToIgnore;
		Parms.DrawDebugType = DrawDebugType;
		Parms.bIgnoreSelf = bIgnoreSelf;
		Parms.TraceColor = TraceColor;
		Parms.TraceHitColor = TraceHitColor;
		Parms.DrawTime = DrawTime;

		this->ProcessEvent(Function, &Parms);

		*OutHit = Parms.OutHit;

		return Parms.ReturnValue;
	}

	//struct FString GetEngineVersion(); // Function Engine.KismetSystemLibrary.GetEngineVersion // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b27f54

	//struct FString GetGameName(); // Function Engine.KismetSystemLibrary.GetGameName // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b28b4c
	FString GetEngineVersion()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetGameName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString ReturnValue;
		} Params = { };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);


			return Params.ReturnValue;
		}

		return FString(L"Error");
	}

	FString GetGameName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetGameName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString ReturnValue;
		} Params = { };

		if (Function)
		{
			this->ProcessEvent(Function, &Params);


			return Params.ReturnValue;
		}

		return FString(L"Error");
	}


	FString GetObjectName(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetObjectName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString GetPathName(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetSystemLibrary.GetPathName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetStringLibrary : public UObject
{
public:

	//struct FString Concat_StrStr(struct FString A, struct FString B); // Function Engine.KismetStringLibrary.Concat_StrStr // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x2ec667c
	FString Concat_StrStr(struct FString A, struct FString B)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.Concat_StrStr");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString A;
			FString B;
			FString ReturnValue;
		} Params = { A, B };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//struct FString TimeSecondsToString(float InSeconds); // Function Engine.KismetStringLibrary.TimeSecondsToString // (Final|RequiredAPI|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x6b7c05c
	FString TimeSecondsToString(float InSeconds)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.TimeSecondsToString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			float InSeconds;
			FString ReturnValue;
		} Params = { InSeconds };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString Conv_IntToString(int32 InInt)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.Conv_IntToString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			int32 InInt;
			FString ReturnValue;
		} Params = { InInt };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FText Conv_StringToText(FString InString)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetTextLibrary.Conv_StringToText");

			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InString;
			FText ReturnValue;
		} Params = { InString };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString Conv_TextToString(FText InText)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetTextLibrary.Conv_TextToString");

			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct {
			FText InText;
			FString ReturnValue;
		} Params = { InText };


		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool Contains(FString SearchIn, FString Substring, bool bUseCase, bool bSearchFromEnd)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.Contains");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString SearchIn;
			FString Substring;
			bool bUseCase;
			bool bSearchFromEnd;
			bool ReturnValue;
		} Params = { SearchIn, Substring, bUseCase, bSearchFromEnd };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FName Conv_StringToName(FString InString)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.Conv_StringToName");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString InString;
			FName ReturnValue;
		} Params = { InString };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString Conv_NameToString(FName InName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.Conv_NameToString");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FName InName;
			FString ReturnValue;
		} Params = { InName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString BuildString_Int(FString AppendTo, FString Prefix, int InInt, FString Suffix)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.BuildString_Int");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString AppendTo;
			FString Prefix;
			int InInt;
			FString Suffix;
			FString ReturnValue;
		} Params = { AppendTo, Prefix, InInt, Suffix };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FString BuildString_Double(FString AppendTo, FString Prefix, double InDouble, FString Suffix)
	{
		static UObject* Function;
		if (!Function)
		{
			auto EncryptedFunction = Encrypt(L"KismetStringLibrary.BuildString_Double");
			Function = FindObjectSingle<UObject*>(EncryptedFunction.decrypt()); EncryptedFunction.clear();
		}

		struct
		{
			FString AppendTo;
			FString Prefix;
			double InDouble;
			FString Suffix;
			FString ReturnValue;
		} Params = { AppendTo, Prefix, InDouble, Suffix };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UEngine : public UObject
{
public:
	UFont* MediumFont()
	{
		return *(UFont**)(this + 0x70);
	}
};

class ULevel : public UObject
{
public:
	TArray<AActor*> Actors()
	{
		return *(TArray<AActor*>*)(this + Offsets::Actors);
	}
};


struct FGameplayTag { char Pad[0x0004]; };
struct FCosmeticCustomizationInfo { char Pad[0x0018]; };
struct FCosmeticLoadoutSlot { char Pad[0x0020]; };
struct FCosmeticLoadout { char Pad[0x0010]; };
struct FFortAthenaLoadout { char Pad[0x0130]; };

struct FGameplayTagContainer final
{
public:
	TArray<struct FGameplayTag>                   GameplayTags;                                      // 0x0000(0x0010)(Edit, BlueprintVisible, ZeroConstructor, EditConst, SaveGame, Protected, NativeAccessSpecifierProtected)
	TArray<struct FGameplayTag>                   ParentTags;                                        // 0x0010(0x0010)(ZeroConstructor, Transient, Protected, NativeAccessSpecifierProtected)
};

struct FCosmeticVariantInfo
{
public:
	struct FGameplayTag                           VariantChannelTag;                                 // 0x0000(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FGameplayTag                           ActiveVariantTag;                                  // 0x0004(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

struct FMcpVariantChannelInfo final : public FCosmeticVariantInfo
{
public:
	struct FGameplayTagContainer                  OwnedVariantTags;                                  // 0x0008(0x0020)(Edit, BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic)
	class UObject* ItemVariantIsUsedFor;                              // 0x0028(0x0008)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString                                 CustomData;                                        // 0x0030(0x0010)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};


class AFortGameState : public UObject
{
public:

};

class AGameStateBase : public UObject
{
public:

};

namespace StaticClasses
{
	static UFortKismetLibrary* FortKismetLibrary = nullptr;
	static UKismetSystemLibrary* KismetSystemLibrary = nullptr;
	static UKismetMathLibrary* KismetMathLibrary = nullptr;
	static UGameplayStatics* GameplayStatics = nullptr;
	static UKismetStringLibrary* KismetStringLibrary = nullptr;
}


class TUObjectArray
{
public:
	enum
	{
		ElementsPerChunk = 0x10000,
	};
public:
	struct FUObjectItem
	{
		class UObject* Object;
		uint8_t Pad[0x10];
	};

	FUObjectItem** Objects;
	uint8_t Pad_0[0x08];
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;

	UObject* GetByIndex(const uint32_t Index) const
	{
		if (Index < 0 || Index > this->NumElements)
			return nullptr;

		const uint32_t ChunkIndex = Index / ElementsPerChunk;
		const uint32_t InChunkIdx = Index % ElementsPerChunk;

		return this->Objects[ChunkIndex][InChunkIdx].Object;
	}

}; TUObjectArray* ObjectArray;

class UWorld : public UObject
{
public:


	AGameStateBase* GameState()
	{
		return *(AGameStateBase**)(this + Offsets::GameState);
	}

	UGameInstance* OwningGameInstance()
	{
		if (this)
		{
			if (UGameInstance* GameInstance = StaticClasses::GameplayStatics->GetGameInstance(this))
				return GameInstance;

			if (this + Offsets::GameInstance)
			{
				return *(UGameInstance**)(this + Offsets::GameInstance);
			}



			return nullptr;
			//return StaticClasses::GameplayStatics->GetGameInstance(this);
		}

		return nullptr;

		return *(UGameInstance**)(this + Offsets::GameInstance);
	}

	TArray<ULevel*> Levels()
	{
		return *(TArray<ULevel*>*)(this + Offsets::Levels);
	}
};
