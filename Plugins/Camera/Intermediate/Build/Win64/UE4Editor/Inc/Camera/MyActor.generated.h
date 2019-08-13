// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef CAMERA_MyActor_generated_h
#error "MyActor.generated.h already included, missing '#pragma once' in MyActor.h"
#endif
#define CAMERA_MyActor_generated_h

#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execLaunchTCP) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=P_THIS->LaunchTCP(); \
		P_NATIVE_END; \
	}


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execLaunchTCP) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=P_THIS->LaunchTCP(); \
		P_NATIVE_END; \
	}


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyActor(); \
	friend struct Z_Construct_UClass_AMyActor_Statics; \
public: \
	DECLARE_CLASS(AMyActor, AActor, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Camera"), NO_API) \
	DECLARE_SERIALIZER(AMyActor)


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS \
private: \
	static void StaticRegisterNativesAMyActor(); \
	friend struct Z_Construct_UClass_AMyActor_Statics; \
public: \
	DECLARE_CLASS(AMyActor, AActor, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Camera"), NO_API) \
	DECLARE_SERIALIZER(AMyActor)


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMyActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMyActor) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyActor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyActor); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMyActor(AMyActor&&); \
	NO_API AMyActor(const AMyActor&); \
public:


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMyActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMyActor(AMyActor&&); \
	NO_API AMyActor(const AMyActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMyActor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMyActor); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMyActor)


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET
#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_14_PROLOG
#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS_NO_PURE_DECLS \
	wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID wrapper_Plugins_Camera_Source_Camera_Public_MyActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
