// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
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

#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS
#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS
#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMyActor(); \
	friend struct Z_Construct_UClass_AMyActor_Statics; \
public: \
	DECLARE_CLASS(AMyActor, AActor, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Camera"), NO_API) \
	DECLARE_SERIALIZER(AMyActor)


#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS \
private: \
	static void StaticRegisterNativesAMyActor(); \
	friend struct Z_Construct_UClass_AMyActor_Statics; \
public: \
	DECLARE_CLASS(AMyActor, AActor, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Camera"), NO_API) \
	DECLARE_SERIALIZER(AMyActor)


#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_STANDARD_CONSTRUCTORS \
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


#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_ENHANCED_CONSTRUCTORS \
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


#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET
#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_14_PROLOG
#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_PRIVATE_PROPERTY_OFFSET \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_INCLASS_NO_PURE_DECLS \
	VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> CAMERA_API UClass* StaticClass<class AMyActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID VirtualAppPlugin_Plugins_Camera_Source_Camera_Public_MyActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
