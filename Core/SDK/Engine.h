#pragma once

// STRUCTS

template <class ArrayType>
struct TArray
{
	TArray()
	{
		this->Data = nullptr;
		this->NumElements = this->MaxElements = 0;
	};

	ArrayType* Data;
	int NumElements, MaxElements;

	int32 GetSlack() const { return this->MaxElements - this->NumElements; }

	ArrayType& operator[](int i)
	{
		return this->Data[i];
	};

	int ESize()
	{
		return this->NumElements;
	}

	bool EValid(int i)
	{
		return bool(i < this->NumElements);
	}


	bool IsValidIndex(int i)
	{
		return bool(i < this->NumElements);
	}

	bool IsValid()
	{
		return this->Data && this->NumElements > 0 && this->MaxElements >= this->NumElements;
	}


	template<size_t N>
	bool New(ArrayType(&elems)[N])
	{
		//Allocate new array
		ArrayType* NewAllocation = new ArrayType[sizeof(ArrayType) * N];

		//Fill with old data
		for (int i = 0; i < this->NumElements - 1; i++)
		{
			NewAllocation[i] = this->Data[i];
		}

		this->NumElements = this->MaxElements = N;

		return true;
	}

	bool Add(ArrayType& Element)
	{
		if (GetSlack() <= 0)
			return false;

		Data[NumElements] = Element;
		NumElements++;

		return true;
	}

	bool Remove(int32 Index)
	{
		if (!Valid(Index))
			return false;

		NumElements--;

		for (int i = Index; i < NumElements; i++)
		{
			/* NumElements was decremented, acessing i + 1 is safe */
			Data[i] = Data[i + 1];
		}

		return true;
	}
};

struct FString : private TArray<wchar_t>
{
	FString() {};

	FString(const wchar_t* Other)
	{
		this->MaxElements = this->NumElements = *Other ? static_cast<int>(NoCRT::__wcslen(Other)) + 1 : 0;

		if (this->NumElements)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	FString(const wchar_t* Other, int Size)
	{
		this->MaxElements = this->NumElements = Size;

		if (this->NumElements)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	operator bool() { return bool(this->Data); }

	wchar_t* c_str()
	{
		return this->Data;
	}

	int ESize()
	{
		return this->NumElements;
	}
};

struct FVector
{
	FVector() : X(), Y(), Z() {}
	FVector(double X, double Y, double Z) : X(X), Y(Y), Z(Z) {}

	FVector operator + (const FVector& other) const { return { this->X + other.X, this->Y + other.Y, this->Z + other.Z }; }
	FVector operator - (const FVector& other) const { return { this->X - other.X, this->Y - other.Y, this->Z - other.Z }; }
	FVector operator * (double offset) const { return { this->X * offset, this->Y * offset, this->Z * offset }; }
	FVector operator / (double offset) const { return { this->X / offset, this->Y / offset, this->Z / offset }; }
	FVector operator * (const FVector& other) const { return { this->X * other.X, this->Y * other.Y, this->Z * other.Z }; }
	FVector operator / (const FVector& other) const { return { this->X / other.X, this->Y / other.Y, this->Z / other.Z }; }

	FVector& operator *= (const double other) { this->X *= other; this->Y *= other; this->Z *= other; return *this; }
	FVector& operator /= (const double other) { this->X /= other; this->Y /= other; this->Z /= other; return *this; }

	FVector& operator = (const FVector& other) { this->X = other.X; this->Y = other.Y; this->Z = other.Z; return *this; }
	FVector& operator += (const FVector& other) { this->X += other.X; this->Y += other.Y; this->Z += other.Z; return *this; }
	FVector& operator -= (const FVector& other) { this->X -= other.X; this->Y -= other.Y; this->Z -= other.Z; return *this; }
	FVector& operator *= (const FVector& other) { this->X *= other.X; this->Y *= other.Y; this->Z *= other.Z; return *this; }
	FVector& operator /= (const FVector& other) { this->X /= other.X; this->Y /= other.Y; this->Z /= other.Z; return *this; }

	operator bool() { return bool(this->X || this->Y || this->Z); }
	friend bool operator == (const FVector& a, const FVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
	friend bool operator != (const FVector& a, const FVector& b) { return !(a == b); }

	double Dot(const FVector& V) { return X * V.X + Y * V.Y + Z * V.Z; }
	double SizeSquared() { return X * X + Y * Y + Z * Z; }

	inline double Distance(FVector v)
	{
		return double(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0) + powf(v.Z - Z, 2.0)));
	}


	bool Normalize()
	{
		while (this->X > 180.0f)
		{
			this->X -= 360.0f;
		}
		while (this->X < -180.0f)
		{
			this->X += 360.0f;
		}
		while (this->Y > 180.0f)
		{
			this->Y -= 360.0f;
		}
		while (this->Y < -180.0f)
		{
			this->Y += 360.0f;
		}

		this->Z = 0;
	}

	void Clamp(float Min, float Max)
	{
		if (this->X < Min)
		{
			this->X = Min;
		}
		else if (this->X > Max)
		{
			this->X = Max;
		}

		if (this->Y < Min)
		{
			this->Y = Min;
		}
		else if (this->Y > Max)
		{
			this->Y = Max;
		}

		this->Z = 0;
	}

	double X, Y, Z;
};

struct FVector2D
{
	FVector2D() : X(), Y() {}
	FVector2D(double X, double Y) : X(X), Y(Y) {}

	FVector2D operator + (const FVector2D& other) const { return { this->X + other.X, this->Y + other.Y }; }
	FVector2D operator - (const FVector2D& other) const { return { this->X - other.X, this->Y - other.Y }; }
	FVector2D operator * (double offset) const { return { this->X * offset, this->Y * offset }; }
	FVector2D operator / (double offset) const { return { this->X / offset, this->Y / offset }; }

	FVector2D& operator *= (const double other) { this->X *= other; this->Y *= other; return *this; }
	FVector2D& operator /= (const double other) { this->X /= other; this->Y /= other; return *this; }

	FVector2D& operator = (const FVector2D& other) { this->X = other.X; this->Y = other.Y; return *this; }
	FVector2D& operator += (const FVector2D& other) { this->X += other.X; this->Y += other.Y; return *this; }
	FVector2D& operator -= (const FVector2D& other) { this->X -= other.X; this->Y -= other.Y; return *this; }
	FVector2D& operator *= (const FVector2D& other) { this->X *= other.X; this->Y *= other.Y; return *this; }
	FVector2D& operator /= (const FVector2D& other) { this->X /= other.X; this->Y /= other.Y; return *this; }

	operator bool() { return bool(this->X || this->Y); }
	friend bool operator == (const FVector2D& A, const FVector2D& B) { return A.X == B.X && A.Y == A.Y; }
	friend bool operator != (const FVector2D& A, const FVector2D& B) { return !(A == B); }

	double X, Y;
};

struct FRotator
{
	FRotator() : Pitch(), Yaw(), Roll() {}
	FRotator(double Pitch, double Yaw, double Roll) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) {}

	FRotator operator + (const FRotator& other) const { return { this->Pitch + other.Pitch, this->Yaw + other.Yaw, this->Roll + other.Roll }; }
	FRotator operator - (const FRotator& other) const { return { this->Pitch - other.Pitch, this->Yaw - other.Yaw, this->Roll - other.Roll }; }
	FRotator operator * (double offset) const { return { this->Pitch * offset, this->Yaw * offset, this->Roll * offset }; }
	FRotator operator / (double offset) const { return { this->Pitch / offset, this->Yaw / offset, this->Roll / offset }; }

	FRotator& operator *= (const double other) { this->Pitch *= other; this->Yaw *= other; this->Roll *= other; return *this; }
	FRotator& operator /= (const double other) { this->Pitch /= other; this->Yaw /= other; this->Roll /= other; return *this; }

	FRotator& operator = (const FRotator& other) { this->Pitch = other.Pitch; this->Yaw = other.Yaw; this->Roll = other.Roll; return *this; }
	FRotator& operator += (const FRotator& other) { this->Pitch += other.Pitch; this->Yaw += other.Yaw; this->Roll += other.Roll; return *this; }
	FRotator& operator -= (const FRotator& other) { this->Pitch -= other.Pitch; this->Yaw -= other.Yaw; this->Roll -= other.Roll; return *this; }
	FRotator& operator *= (const FRotator& other) { this->Pitch *= other.Pitch; this->Yaw *= other.Yaw; this->Roll *= other.Roll; return *this; }
	FRotator& operator /= (const FRotator& other) { this->Pitch /= other.Pitch; this->Yaw /= other.Yaw; this->Roll /= other.Roll; return *this; }

	operator bool() { return bool(this->Pitch || this->Yaw || this->Roll); }
	friend bool operator == (const FRotator& a, const FRotator& b) { return a.Pitch == b.Pitch && a.Yaw == b.Yaw && a.Roll == b.Roll; }
	friend bool operator != (const FRotator& a, const FRotator& b) { return !(a == b); }

	FVector Euler() const
	{
		return FVector(Pitch, Yaw, Roll);
	}

	FRotator Normalize()
	{
		while (this->Yaw > 180.0)
			this->Yaw -= 360.0;
		while (this->Yaw < -180.0)
			this->Yaw += 360.0;

		while (this->Pitch > 180.0)
			this->Pitch -= 360.0;
		while (this->Pitch < -180.0)
			this->Pitch += 360.0;

		this->Roll = 0.0;
		return *this;
	}

	double Pitch, Yaw, Roll;
};

struct FLinearColor final
{
public:
	float                                         R;                                                 // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         G;                                                 // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         B;                                                 // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         A;                                                 // 0x000C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};


// ScriptStruct CoreUObject.Color
// Size: 0x04 (Inherited: 0x00)
struct FColor {
	int B; // 0x00(0x01)
	int G; // 0x01(0x01)
	int R; // 0x02(0x01)
	int A; // 0x03(0x01)
};

struct FPlane : public FVector
{
	FPlane() : W() {}
	FPlane(double W) : W(W) {}

	double W;
};

struct FMatrix
{
	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() {}
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane) : XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) {}

	FPlane XPlane, YPlane, ZPlane, WPlane;
};

struct FName
{
	uint32_t ComparisonIndex;
	uint32_t DisplayIndex;
};

struct FKey
{
	FName KeyName;
	char Details[0x10];
};


class FTextData
{
public:
	char Details[0x20];
	FString String;
};

struct FText
{
	FTextData* Data;

	FString Get()
	{
		if (this->Data)
		{
			return Data->String;
		}

		return nullptr;
	}
};

struct FHitResult {
	unsigned char UnknownData69[0x4];
	int32_t FaceIndex; // 0x00(0x04)
	float Time; // 0x04(0x04)
	float Distance; // 0x08(0x04)
	char pad_C[0x4]; // 0x0c(0x04)
	struct FVector Location; // 0x10(0x18)
	struct FVector ImpactPoint; // 0x28(0x18)
	struct FVector Normal; // 0x40(0x18)
	struct FVector ImpactNormal; // 0x58(0x18)
	struct FVector TraceStart; // 0x70(0x18)
	struct FVector TraceEnd; // 0x88(0x18)
	float PenetrationDepth; // 0xa0(0x04)
	int32_t MyItem; // 0xa4(0x04)
	int32_t Item; // 0xa8(0x04)
	char ElementIndex; // 0xac(0x01)
	char bBlockingHit : 1; // 0xad(0x01)
	char bStartPenetrating : 1; // 0xad(0x01)
	char pad_AD_2 : 6; // 0xad(0x01)
	char pad_AE[0x2]; // 0xae(0x02)
	void* PhysMaterial; // 0xb0(0x08)
	void* HitObjectHandle; // 0xb8(0x20)
	void* Component; // 0xd8(0x08)
	char pad_E0[0x8]; // 0xe0(0x08)
	struct FName BoneName; // 0xe8(0x04)
	struct FName MyBoneName; // 0xec(0x04)
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};


class FWeakObjectPtr
{
public:
	uint32_t                                         ObjectIndex;                                       // 0x0000(0x0004)(NOT AUTO-GENERATED PROPERTY)
	uint32_t                                         ObjectSerialNumber;                                // 0x0004(0x0004)(NOT AUTO-GENERATED PROPERTY)

public:
	class UObject* Get();
	class UObject* operator->() const;
	bool operator==(const FWeakObjectPtr& Other) const;
	bool operator!=(const FWeakObjectPtr& Other) const;
	bool operator==(const class UObject* Other) const;
	bool operator!=(const class UObject* Other) const;
};

template<typename UEType>
class TWeakObjectPtr : public FWeakObjectPtr
{
public:
	UEType* Get()
	{
		return static_cast<UEType*>(FWeakObjectPtr::Get());
	}

	UEType* operator->()
	{
		return static_cast<UEType*>(FWeakObjectPtr::Get());
	}
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr                                WeakPtr;                                           // 0x0000(0x0008)(NOT AUTO-GENERATED PROPERTY)
	TObjectID                                     ObjectID;                                          // 0x0008(0x0000)(NOT AUTO-GENERATED PROPERTY)

public:
	class UObject* Get()
	{
		//return WeakPtr.Get();
	}
	class UObject* operator->() const
	{
		//return WeakPtr.Get();
	}
};

struct FSoftObjectPath
{
public:
	FName AssetPathName;
	FString SubPathString;
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
};

template<typename UEType>
class TSoftObjectPtr : public FSoftObjectPtr
{
public:
	UEType* Get()
	{
		return static_cast<UEType*>(TPersistentObjectPtr::Get());
	}
	UEType* operator->() const
	{
		return static_cast<UEType*>(TPersistentObjectPtr::Get());
	}
};

struct FGuid
{
	int32_t A; // 0x00(0x04)
	int32_t B; // 0x04(0x04)
	int32_t C; // 0x08(0x04)
	int32_t D; // 0x0c(0x04)
};

struct FRankedProgressReplicatedData
{
	FString RankType; // 0x00(0x10)
	int32_t Rank; // 0x10(0x04)
	char pad_14[0x4]; // 0x14(0x04)
};

struct FBatchedThickLines
{
	FVector Start;
	FVector End;
	float Thickness;
	FLinearColor Color;
	FLinearColor HitProxyColor;
	float DepthBias;
	uint32 bScreenSpace;
};

// ENUMS

enum class EFortWeaponCoreAnimation : uint8
{
	Melee = 0,
	Pistol = 1,
	Shotgun = 2,
	PaperBlueprint = 3,
	Rifle = 4,
	MeleeOneHand = 5,
	MachinePistol = 6,
	RocketLauncher = 7,
	GrenadeLauncher = 8,
	GoingCommando = 9,
	AssaultRifle = 10,
	TacticalShotgun = 11,
	SniperRifle = 12,
	TrapPlacement = 13,
	ShoulderLauncher = 14,
	AbilityDecoTool = 15,
	Crossbow = 16,
	C4 = 17,
	RemoteControl = 18,
	DualWield = 19,
	AR_BullPup = 20,
	AR_ForwardGrip = 21,
	MedPackPaddles = 22,
	SMG_P90 = 23,
	AR_DrumGun = 24,
	Consumable_Small = 25,
	Consumable_Large = 26,
	Balloon = 27,
	MountedTurret = 28,
	CreativeTool = 29,
	ExplosiveBow = 30,
	AshtonIndigo = 31,
	AshtonChicago = 32,
	MeleeDualWield = 33,
	Unarmed = 34,
	MAX = 35
};

enum class EFortItemTier : uint8
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12
};

enum EFortRarity : uint8_t
{
	Common = 0,
	Uncommon = 1,
	Rare = 2,
	Epic = 3,
	Legendary = 4,
	Mythic = 5,
	Transcendent = 6,
	Unattainable = 7,
	NumRarityValues = 8,
	EFortRarity_MAX = 9
};

enum class ECollisionChannel : uint8_t
{
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

enum class ETraceTypeQuery : uint8
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33,
};

enum class EDrawDebugTrace : uint8
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4,
};

enum class EBlendMode : uint8
{
	BLEND_Opaque = 0,
	BLEND_Masked = 1,
	BLEND_Translucent = 2,
	BLEND_Additive = 3,
	BLEND_Modulate = 4,
	BLEND_AlphaComposite = 5,
	BLEND_AlphaHoldout = 6,
	BLEND_TranslucentColoredTransmittance = 7,
	BLEND_MAX = 8,
	BLEND_TranslucentGreyTransmittance = 2,
	BLEND_ColoredTransmittanceOnly = 4
};

enum class EDecalBlendMode : uint8
{
	DBM_Translucent = 0,
	DBM_Stain = 1,
	DBM_Normal = 2,
	DBM_Emissive = 3,
	DBM_DBuffer_ColorNormalRoughness = 4,
	DBM_DBuffer_Color = 5,
	DBM_DBuffer_ColorNormal = 6,
	DBM_DBuffer_ColorRoughness = 7,
	DBM_DBuffer_Normal = 8,
	DBM_DBuffer_NormalRoughness = 9,
	DBM_DBuffer_Roughness = 10,
	DBM_DBuffer_Emissive = 11,
	DBM_DBuffer_AlphaComposite = 12,
	DBM_DBuffer_EmissiveAlphaComposite = 13,
	DBM_Volumetric_DistanceFunction = 14,
	DBM_AlphaComposite = 15,
	DBM_AmbientOcclusion = 16,
	DBM_MAX = 17
};

enum class EElementType
{
	ET_Box,
	ET_DebugQuad,
	ET_Text,
	ET_ShapedText,
	ET_Spline,
	ET_Line,
	ET_Gradient,
	ET_Viewport,
	ET_Border,
	ET_Custom,
	ET_CustomVerts,
	ET_PostProcessPass,
	ET_Count,
};

enum class ESimpleElementBlendMode
{
	SE_BLEND_Opaque = 0,
	SE_BLEND_Masked,
	SE_BLEND_Translucent,
	SE_BLEND_Additive,
	SE_BLEND_Modulate,
	SE_BLEND_MaskedDistanceField,
	SE_BLEND_MaskedDistanceFieldShadowed,
	SE_BLEND_TranslucentDistanceField,
	SE_BLEND_TranslucentDistanceFieldShadowed,
	SE_BLEND_AlphaComposite,
	SE_BLEND_AlphaHoldout,
	SE_BLEND_AlphaBlend,
	SE_BLEND_TranslucentAlphaOnly,
	SE_BLEND_TranslucentAlphaOnlyWriteAlpha,
	SE_BLEND_RGBA_MASK_START,
	SE_BLEND_RGBA_MASK_END = SE_BLEND_RGBA_MASK_START + 31,
	SE_BLEND_MAX,
};

enum class ENetworkSmoothingMode : uint8_t
{
	Disabled,
	Linear,
	Exponential,
	Replay,
};

enum class EAutoPossessAI : uint8
{
	Disabled = 0,
	PlacedInWorld = 1,
	Spawned = 2,
	PlacedInWorldOrSpawned = 3,
	EAutoPossessAI_MAX = 4,
};

enum class EMovementMode : uint8
{
	MOVE_None = 0,
	MOVE_Walking = 1,
	MOVE_NavWalking = 2,
	MOVE_Falling = 3,
	MOVE_Swimming = 4,
	MOVE_Flying = 5,
	MOVE_Custom = 6,
	MOVE_MAX = 7,
};

enum class ENetDormancy : uint8
{
	DORM_Never = 0,
	DORM_Awake = 1,
	DORM_DormantAll = 2,
	DORM_DormantPartial = 3,
	DORM_Initial = 4,
	DORM_MAX = 5,
};

enum class EObjectFlags : uint32_t
{
	RF_NoFlags = 0,
	RF_Public = 1,
	RF_Standalone = 2,
	RF_MarkAsNative = 4,
	RF_Transactional = 8,
	RF_ClassDefaultObject = 16,
	RF_ArchetypeObject = 32,
	RF_Transient = 64,
	RF_MarkAsRootSet = 128,
	RF_TagGarbageTemp = 256,
	RF_NeedInitialization = 512,
	RF_NeedLoad = 1024,
	RF_KeepForCooker = 2048,
	RF_NeedPostLoad = 4096,
	RF_NeedPostLoadSubobjects = 8192,
	RF_NewerVersionExists = 16384,
	RF_BeginDestroyed = 32768,
	RF_FinishDestroyed = 65536,
	RF_BeingRegenerated = 131072,
	RF_DefaultSubObject = 262144,
	RF_WasLoaded = 524288,
	RF_TextExportTransient = 1048576,
	RF_LoadCompleted = 2097152,
	RF_InheritableComponentTemplate = 4194304,
	RF_DuplicateTransient = 8388608,
	RF_StrongRefOnFrame = 16777216,
	RF_NonPIEDuplicateTransient = 33554432,
	RF_Dynamic = 67108864,
	RF_WillBeLoaded = 134217728
};

enum class EFortKickReason : uint8
{
	NotKicked = 0,
	GenericKick = 1,
	WasBanned = 2,
	EncryptionRequired = 3,
	CrossPlayRestriction = 4,
	ClientIdRestriction = 5,
	EFortKickReason_MAX = 6
};

enum class EFortBuildingType : uint8
{
	Wall = 0,
	Floor = 1,
	Corner = 2,
	Deco = 3,
	Prop = 4,
	Stairs = 5,
	Roof = 6,
	Pillar = 7,
	SpawnedItem = 8,
	Container = 9,
	Trap = 10,
	GenericCenterCellActor = 11,
	StructuralCorner = 12,
	None = 13,
	EFortBuildingType_MAX = 14,
};