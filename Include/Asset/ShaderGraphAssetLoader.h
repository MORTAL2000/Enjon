// Copyright 2016-2017 John Jackson. All Rights Reserved.
// File: ShaderGraphAssetLoader.h

#pragma  once
#ifndef ENJON_SHADER_GRAPH_ASSET_LOADER_H
#define ENJON_SHADER_GRAPH_ASSET_LOADER_H 

#include "Asset/AssetLoader.h"
#include "Graphics/ShaderGraph.h"

namespace Enjon
{
	ENJON_CLASS()
	class ShaderGraphAssetLoader : public AssetLoader
	{
		ENJON_CLASS_BODY()

		public:

			/**
			* @brief Constructor
			*/
			ShaderGraphAssetLoader( );

			/**
			* @brief Destructor
			*/
			~ShaderGraphAssetLoader( );


		protected:

			/**
			* @brief
			*/
			virtual void RegisterDefaultAsset( ) override;


		private:
			/**
			* @brief
			*/
			Asset* LoadResourceFromFile( const String& filePath, const String& name ) override;

			/**
			* @brief
			*/
			ShaderGraph* LoadShaderGraphFromFile( const Enjon::String& filePath );
	};
}

#endif