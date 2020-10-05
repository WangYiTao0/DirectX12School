#pragma once

#include "main.h"
#include "CTexture.h"

using namespace Microsoft::WRL;

class CPolygon
{
private:
	ComPtr<ID3D12Resource> m_VertexBuffer;
	ComPtr<ID3D12Resource> m_ConstantBuffer;

	std::unique_ptr<CTexture> m_texture;
public : 
	void Initialized();
	void UnInitialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdlist);
};