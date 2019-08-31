// @file EditorApp.h
// Copyright 2016-2018 John Jackson. All Rights Reserved.

#pragma once 
#ifndef ENJON_EDITOR_H
#define ENJON_EDITOR_H

#include "Project.h"
#include "EditorTransformWidget.h"
#include "EditorView.h"
#include "EditorSceneView.h"
#include "EditorInspectorView.h"
#include "EditorObject.h"

#include <Application.h>
#include <Entity/EntityManager.h>
#include <Graphics/Renderable.h> 
#include <Graphics/Camera.h>
#include <Scene/Scene.h>
#include <Base/World.h> 

namespace Enjon
{ 
	using ReloadDLLCallback = std::function< void( void ) >;

	class StaticMeshRenderable;
	class EditorAssetBrowserView;
	class EditorViewport;
	class EditorWindow;
	class EditorMaterialEditWindow; 
	class EditorWorldOutlinerView;
	class EditorArchetypeEditWindow; 
	class EditorTransformWidgetToolBar;

	enum class TransformMode
	{
		Translate,
		Scale,
		Rotate
	};

	enum class BuildSystemType
	{
		VS2015,
		VS2017,
		VS2019,
		Count
	};

	typedef struct BuildSystemOption
	{
		BuildSystemOption( ) = default;
		BuildSystemOption( const char* name, const char* flags )
			: mName( name ), mCMakeFlags( flags )
		{ 
		}

		const char* mName = nullptr;
		const char* mCMakeFlags = nullptr;
	} BuildSystemOption; 

	// Possibly want different build system options that I can use easier than what I'm doing here...

	// This doesn't necessarily make sense...
	ENJON_CLASS( Construct )
	class BuildSystemSettings : public Object
	{
		ENJON_CLASS_BODY( BuildSystemSettings )

		public:


		public:
			ENJON_PROPERTY( )
			String mName;

			ENJON_PROPERTY( )
			String mCompilerDirectory; 

			ENJON_PROPERTY( )
			String mCMakeFlags; 
	};

	// Possibly something like this?
	class VS2015BuildSystemSettings : public BuildSystemSettings
	{ 

	};

	ENJON_CLASS( Construct )
	class EditorConfigSettings : public Object
	{
		ENJON_CLASS_BODY( EditorConfigSettings )

		/*
			Config settings need: 
				- Build system information:
					- Name
					- Compiler Directory
					- CMake Flags
				- Recent project list ( manifest )
					- Project names
					- Project directories 
				- Various editor settings for user config
		*/	

		public:
			ENJON_PROPERTY( )
			BuildSystemSettings mBuildSystemSettings;

			ENJON_PROPERTY( )
			Vector< Project > mProjectList;
	};

	// TODO(john): Need to reflect over the editor app to get introspection meta data
	ENJON_CLASS( Construct )
	class EditorApp : public Application
	{
		ENJON_MODULE_BODY( EditorApp )

		public:

			virtual Enjon::Result Initialize() override;  

			/**
			* @brief Main update tick for application.
			* @return Enjon::Result
			*/
			virtual Enjon::Result Update(f32 dt) override;

			/**
			* @brief Processes input from input class 
			* @return Enjon::Result
			*/
			virtual Enjon::Result ProcessInput(f32 dt) override;

			/**
			* @brief Shuts down application and cleans up any memory that was allocated.
			* @return Enjon::Result
			*/
			virtual Enjon::Result Shutdown() override; 

			/**
			* @brief 
			*/
			void RegisterReloadDLLCallback( const ReloadDLLCallback& callback ); 

			/**
			* @brief 
			*/
			Vec2 GetSceneViewProjectedCursorPosition( ); 

			/**
			* @brief 
			*/
			EditorInspectorView* GetInspectorView( );

			/**
			* @brief 
			*/
			EditorAssetBrowserView* GetEditorAssetBrowserView( );

			/**
			* @brief 
			*/
			String GetBuildConfig( ) const;
 
			/**
			* @brief 
			*/
			String GetVisualStudioDirectoryPath( ) const;

			String GetProjectMainTemplate( ) const;

			String GetCompileProjectCMakeTemplate( ) const;

			String GetBuildAndRunCompileTemplate( ) const;

			String GetProjectEnjonDefinesTemplate( ) const;

			Project* GetProject( );

			EntityHandle GetSelectedEntity( );

			void SelectEntity( const EntityHandle& handle );

			void DeselectEntity( );

			void EnableOpenNewComponentDialogue( );
			void OpenNewComponentDialogue( ); 
			void AddComponentPopupView( );

			void SetProjectOnLoad( const String& projectDir );

			void EnableTransformSnapping( bool enable, const TransformationMode& mode );
			bool IsTransformSnappingEnabled( const TransformationMode& mode );

			f32 GetTransformSnap( const TransformationMode& mode );
			void SetTransformSnap( const TransformationMode& mode, const f32& val );

			EditorTransformWidget* GetTransformWidget( );

		public:
			Vec4 mRectColor = Vec4( 0.8f, 0.3f, 0.1f, 1.0f );

		private:
			void CreateComponent( const String& componentName );
			void LoadResourceFromFile( );
			void PlayOptions( );
			void CameraOptions( bool* enable );
			bool CreateProjectView( );
			void SelectSceneView( );
			void SelectProjectDirectoryView( ); 
			void ProjectListView( ); 
			void NewProjectView( );

			void LoadProjectView( );
			void CheckForPopups( );

			void CreateNewProject( const String& projectName );


			void LoadProject( const Project& project );
			bool UnloadDLL( ByteBuffer* buffer = nullptr );
			void LoadDLL( bool releaseSceneAsset = true );
			void ReloadDLL( );

			void WriteEditorConfigFileToDisk( );
			void CollectAllProjectsOnDisk( );
			void LoadProjectSolution( );

			void CleanupScene( );

			void InitializeProjectApp( );
			void ShutdownProjectApp( ByteBuffer* buffer );

			void LoadResources( ); 
			void LoadProjectResources( ); 

			void InspectorView( bool* enabled );
			void SceneView( bool* viewBool ); 

			void LoadProjectContext( );
			void LoadProjectSelectionContext( ); 
			void UnloadPreviousProject( );
			void FindBuildSystem( );

			void PreloadProject( const Project& project );

			void CleanupGUIContext( );

			void PreCreateNewProject( const String& projectName );

			void FindProjectOnLoad( );

			void DeserializeEditorConfigSettings( ); 
			void SerializeEditorConfigSettings( );

			void BuildSystemView( );
			
			void InitializeBuildSystemOptions( );

		private:
			bool mViewBool = true;
			bool mShowCameraOptions = true;
			bool mShowLoadResourceOption = true;
			bool mShowCreateProjectView = true;
			bool mShowSceneView = true;
			Enjon::String mResourceFilePathToLoad = "";
			bool mMoveCamera = false; 
			bool mNewComponentPopupDialogue = false;
			bool mLoadProjectPopupDialogue = false;
			bool mPreloadProjectContext = false;
			bool mPrecreateNewProject = false; 

			bool mPlaying = false;
			bool mNeedsStartup = true; 
			bool mNeedsShutdown = false;
			bool mNeedsLoadProject = false;
			bool mNeedRecompile = false;
			bool mNeedReload = false;

			Enjon::f32 mCameraSpeed = 10.f;
			Enjon::f32 mMouseSensitivity = 10.0f;
			Enjon::Transform mPreviousCameraTransform;

			Project mProject;

			String mNewProjectName = "NewProject";
 
			String mProjectsPath = "";
			String mProjectSourceTemplate = "";
			String mProjectCMakeTemplate = "";
			String mProjectDelBatTemplate = "";
			String mProjectBuildAndRunTemplate = "";
			String mProjectBuildAndRunCompileTemplate = "";
			String mComponentSourceTemplate = "";
			String mCompileProjectBatTemplate = "";
			String mCompileProjectCMakeTemplate = "";
			String mProjectMainTemplate = "";
			String mProjectEnjonDefinesTemplate = "";
			String mProjectBuildBatTemplate = "";

			String mProjectOnLoad = "";

			Vector<Project> mProjectsOnDisk;
			Vector<Entity*> mSceneEntities;

			EntityHandle mSceneEntity; 

			EditorTransformWidget mTransformWidget;

			Vec2 mSceneViewWindowPosition;
			Vec2 mSceneViewWindowSize;
			EntityHandle mSelectedEntity;

			// This could get dangerous...
			AssetHandle<Scene> mCurrentScene; 

			EditorViewport* mEditorSceneView = nullptr;
			//EditorSceneView* mEditorSceneView = nullptr; 
			EditorInspectorView* mInspectorView = nullptr; 
			EditorAssetBrowserView* mAssetBroswerView = nullptr; 
			EditorWorldOutlinerView* mWorldOutlinerView = nullptr; 
			EditorArchetypeEditWindow* mArchetypeWindow = nullptr;
			EditorTransformWidgetToolBar* mTransformToolBar = nullptr;

			Camera mEditorCamera;
			Vec3 mCameraRotator = Vec3( 0.0f ); 

			Vector< ReloadDLLCallback > mReloadDLLCallbacks;

			Window* mProjectSelectionWindow = nullptr;

			ENJON_PROPERTY( HideInEditor )
			EditorConfigSettings mConfigSettings;

			BuildSystemOption mBuildSystemOptions[ (u32)BuildSystemType::Count ];
	}; 

	// Declaration for module export
	ENJON_MODULE_DECLARE( EditorApp ) 

}

#endif
