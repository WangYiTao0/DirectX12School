#pragma once
#include "main.h"
#include "CTexture.h"

using namespace Microsoft::WRL;

class CCube
{
private:
	ComPtr<ID3D12Resource> m_VertexBuffer;
	ComPtr<ID3D12Resource> m_IndexBuffer;
	ComPtr<ID3D12Resource> m_ConstantBuffer;
	std::unique_ptr<CTexture> m_texture;

	XMFLOAT3 m_Rotation;
public:
	void Initialized();
	void Uninitialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdlist);
};