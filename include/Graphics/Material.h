#pragma once
#ifndef ENJON_MATERIAL_H
#define ENJON_MATERIAL_H 

#include "Graphics/GLTexture.h"
#include "Graphics/Texture.h"
#include "Graphics/Color.h"
#include "Graphics/Shader.h"
#include "System/Types.h"
#include "Asset/Asset.h"

#include <unordered_map>

namespace Enjon { 

	class GLSLProgram;

	enum class TextureSlotType
	{
		Albedo,
		Normal,
		Emissive,
		Metallic,
		Roughness,
		AO,
		Count
	};

	class Material : public Asset
	{
		ENJON_OBJECT( Material )

		public:
			Material();
			~Material(); 

			void SetTexture(const TextureSlotType& type, const AssetHandle<Texture>& textureHandle);
			AssetHandle<Texture> GetTexture(const TextureSlotType& type) const;

			void SetColor(TextureSlotType type, const ColorRGBA16& color);
			ColorRGBA16 GetColor(TextureSlotType type) const;

			GLSLProgram* GetShader();
			void SetShader(GLSLProgram* shader);

			bool TwoSided( ) const { return mTwoSided; }
			void TwoSided( bool enable ) { mTwoSided = enable; }

		private:
			AssetHandle<Texture> mTextureHandles[(u32)TextureSlotType::Count]; 
			ColorRGBA16 mColors[(u32)TextureSlotType::Count];
			GLSLProgram* mShader = nullptr;
			bool mTwoSided = false;
			std::unordered_map< Enjon::String, ShaderUniform* > mUniforms;
	}; 
}


#endif